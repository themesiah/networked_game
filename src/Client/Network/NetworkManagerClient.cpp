#include "NetworkManagerClient.h"

#include <fstream>

#include "../Engine/Engine.h"
#include "Replication\ReplicationManager.h"

#include "Socket\SocketAddress.h"
#include "Socket\SocketAddressFactory.h"
#include "Replication\Packet.h"
#include "Replication\ObjectCreationRegistry.h"

#include "Movement.h"

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
	SET_REFLECTION_DATA(CPlayerController);
	SET_REFLECTION_DATA(CMovement);
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
			OutputMemoryBitStream l_Output;
			l_Output.Serialize(PT_Hello, PACKET_BIT_SIZE);
			// TODO: Player name!
			l_Output.WriteSize();
			int sent = m_Socket->Send(l_Output.GetBufferPtr(), l_Output.GetByteLength());
			m_State = ClientState::HELLO_SENT;
		}
		break;
	case ClientState::PLAYING:
		{
			CMovement* lMovement = CEngine::GetInstance().GetMovement();
			OutputMemoryBitStream l_Output;
			l_Output.Serialize(PT_ReplicationData, PACKET_BIT_SIZE);
			lMovement->SerializeWrite(l_Output);
			l_Output.WriteSize();
			int sent = m_Socket->Send(l_Output.GetBufferPtr(), l_Output.GetByteLength());
			lMovement->Reset();
		}
		break;
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
		InputMemoryBitStream l_Input(p.buffer, p.size);
		uint8_t packetType;
		l_Input.Serialize(packetType, PACKET_BIT_SIZE);
		if (packetType == PacketType::PT_ReplicationData && m_State == ClientState::PLAYING) {
			auto receivedGameObjects = lReplicationManager.ReceiveReplicatedObjects(l_Input);
			lGameObjects->swap(receivedGameObjects);
		}
		else if (packetType == PacketType::PT_Hello && m_State == ClientState::HELLO_SENT) {
			m_State = ClientState::PLAYING;
		}
		std::free(p.buffer);
		p = m_PacketStream.ReadPacket();
	}
}

void CNetworkManagerClient::ManageDisconnection()
{
	OutputMemoryBitStream l_Output;
	l_Output.Serialize(PT_Disconnect, PACKET_BIT_SIZE);
	l_Output.WriteSize();
	m_Socket->Send(l_Output.GetBufferPtr(), l_Output.GetByteLength());
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