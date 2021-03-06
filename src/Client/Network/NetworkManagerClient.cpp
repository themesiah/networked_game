#include "NetworkManagerClient.h"

#include <fstream>

#include "../Engine/Engine.h"
#include "Replication\ReplicationManager.h"
#include "Replication\LinkingContext.h"
#include "../Graphics/CameraController.h"
#include "../Engine/GUIManager.h"
#include "Common\RPCManager.h"

#include "Socket\SocketAddress.h"
#include "Socket\SocketAddressFactory.h"
#include "Replication\Packet.h"
#include "Replication\ObjectCreationRegistry.h"

#include "CommonClasses\Movement.h"
#include "../Graphics/TilemapClient.h"
#include "../Controllers/PlayernameClient.h"
#include "../Controllers/PlayerControllerClient.h"

#include "imgui.h"
#include "imgui-SFML.h"

CNetworkManagerClient::CNetworkManagerClient() :
m_AverageTimeBetweenPackets(0.1f)
, m_NumberOfSamples(1)
, m_Timer(0.f)
, m_State(ClientState::NOT_CONNECTED)
{
	
}

CNetworkManagerClient::~CNetworkManagerClient()
{
	
}

bool CNetworkManagerClient::Init(uint16_t aPort)
{
	return NetworkManager::Init(aPort);
}

bool CNetworkManagerClient::InitReflection()
{
	SET_REFLECTION_DATA(CPlayerControllerClient);
	SET_REFLECTION_DATA(CMovement);
	SET_REFLECTION_DATA(TilemapClient);
	SET_REFLECTION_DATA(PlayernameClient);
	return true;
}

bool CNetworkManagerClient::Connect()
{
	std::string lAddressString = "localhost:6900";
	std::ifstream file("Data/server.txt");
	if (file.is_open())
	{
		std::getline(file, lAddressString);
		file.close();
	}
	else {
		return false;
	}
	SocketAddressPtr sendingAddress = SocketAddressFactory::CreateIPv4FromString(lAddressString);
	if (m_Socket->Connect(*sendingAddress.get()) != NO_ERROR)
	{
		return false;
	}
	m_Socket->SetNonBlockingMode(true);
	m_State = ClientState::CONNECTED;
	return true;
}

void CNetworkManagerClient::UpdateSendingSockets(float aDeltaTime)
{
	switch (m_State)
	{
	case ClientState::CONNECTED:
		{
			CGUIManager::LoginData lLoginData = CEngine::GetInstance().GetGUIManager().LoginGUI();
			if (lLoginData.success == true) {
				OutputMemoryBitStream lOutput;
				lOutput.Serialize(PacketType::PT_Hello, PACKET_BIT_SIZE);
				lOutput.Serialize(lLoginData.name);
				lOutput.Serialize(lLoginData.graphicId);
				lOutput.Close();
				int sent = m_Socket->Send(lOutput.GetBufferPtr(), lOutput.GetByteLength());
				m_State = ClientState::HELLO_SENT;
			}
		}
		break;
	case ClientState::PLAYING:
		{
			CMovement* lMovement = CEngine::GetInstance().GetMovement();
			if (lMovement->GetDirty() > 0) {
				OutputMemoryBitStream lOutput;
				lOutput.Serialize(PacketType::PT_RPC, PACKET_BIT_SIZE);
				lOutput.Serialize<uint32_t>('MOVE');
				lMovement->SerializeWrite(lOutput);
				lOutput.Close();
				int sent = m_Socket->Send(lOutput.GetBufferPtr(), lOutput.GetByteLength());
				lMovement->Undirty();
			}

			OutputMemoryBitStream lRPCOutput;
			if (CEngine::GetInstance().GetGUIManager().DebugGUI(lRPCOutput)) {
				lRPCOutput.Close();
				int sent = m_Socket->Send(lRPCOutput.GetBufferPtr(), lRPCOutput.GetByteLength());
			}
			
		}
		break;
	case ClientState::PENDING_DISCONNECTION:
	{
		CEngine::GetInstance().GetGUIManager().DisconnectionMessage(m_Reason);
		break;
	}
	default: // HELLO_SENT and NOT_CONNECTED
		break;
	}
}

void CNetworkManagerClient::UpdateReceivingSockets(float aDeltaTime)
{
	char segment[SEGMENT_SIZE];
	FD_ZERO(segment);
	m_Timer += aDeltaTime;
	int dataReceived = m_Socket->Receive(segment, SEGMENT_SIZE);
	if (dataReceived > 0) {
		AddSample(m_Timer);
		m_Timer = 0.f;
		m_PacketStream.WriteBytes(segment, dataReceived);
	}
}

void CNetworkManagerClient::AddSample(float aSample)
{
	float lAverageSum = m_AverageTimeBetweenPackets * (float)m_NumberOfSamples;
	lAverageSum += std::min<float>(aSample, 1.f);
	m_NumberOfSamples++;
	m_AverageTimeBetweenPackets = lAverageSum / (float)m_NumberOfSamples;
}

void CNetworkManagerClient::UpdatePackets(float aDeltaTime)
{
	auto lGameObjects = CEngine::GetInstance().GetGameObjects();
	CReplicationManager& lReplicationManager = CEngine::GetInstance().GetReplicationManager();
	PacketStream::Packet p;
	p = m_PacketStream.ReadPacket();
	while (p.size > 0)
	{
		InputMemoryBitStream lInput(p.buffer, p.size);
		uint8_t packetType;
		lInput.Serialize(packetType, PACKET_BIT_SIZE);
		if (packetType == PacketType::PT_ReplicationData && m_State == ClientState::PLAYING) {
			auto receivedGameObjects = lReplicationManager.ReceiveReplicatedObjects(lInput);
			lGameObjects->swap(receivedGameObjects);
		}
		else if (packetType == PacketType::PT_ReplicationDeltas && m_State == ClientState::PLAYING) {
			auto receivedGameObjects = lReplicationManager.ReceiveReplicatedDeltas(lInput);
			lGameObjects->insert(receivedGameObjects.begin(), receivedGameObjects.end());
			//lGameObjects->swap(receivedGameObjects);
		}
		else if (packetType == PacketType::PT_RPC && m_State == ClientState::PLAYING) {
			CEngine::GetInstance().GetRPCManager().ProcessRPC(lInput);
		}
		else if (packetType == PacketType::PT_Hello && m_State == ClientState::HELLO_SENT) {
			m_State = ClientState::PLAYING;
			uint32_t lNetworkId;
			lInput.Serialize(lNetworkId);
			CEngine::GetInstance().GetCameraController().SetFollow(lNetworkId);
		}
		else if (packetType == PacketType::PT_Disconnect) {
			m_State = ClientState::PENDING_DISCONNECTION;
			lInput.Serialize(m_Reason);
		}
		std::free(p.buffer);
		p = m_PacketStream.ReadPacket();
	}
}

void CNetworkManagerClient::RPCSend(OutputMemoryBitStream& lOutput)
{
	m_Socket->Send(lOutput.GetBufferPtr(), lOutput.GetByteLength());
}

void CNetworkManagerClient::ManageDisconnection()
{
	if (m_State != ClientState::PENDING_DISCONNECTION)
	{
		OutputMemoryBitStream lOutput;
		uint8_t packetType = PacketType::PT_Disconnect;
		lOutput.Serialize(packetType, PACKET_BIT_SIZE);
		lOutput.Close();
		m_Socket->Send(lOutput.GetBufferPtr(), lOutput.GetByteLength());
		m_State = ClientState::PENDING_DISCONNECTION;
	}
}

void CNetworkManagerClient::RenderImGui()
{
	ImGui::Text("Avg: %f", m_AverageTimeBetweenPackets);
	switch (m_State)
	{
	case ClientState::NOT_CONNECTED:
		ImGui::Text("State: %s", "Not connected");
		break;
	case ClientState::CONNECTED:
		ImGui::Text("State: %s", "Connected");
		break;
	case ClientState::HELLO_SENT:
		ImGui::Text("State: %s", "Hello sent");
		break;
	case ClientState::PENDING_DISCONNECTION:
		ImGui::Text("State: %s", "Pending disconnection");
		break;
	case ClientState::PLAYING:
		ImGui::Text("State: %s", "Playing");
		break;
	}
}