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
			m_OutputMs->Reset();
			uint8_t packetType = PacketType::PT_Hello;
			((MemoryStream*)m_OutputMs)->Serialize(packetType, PACKET_BIT_SIZE);
			// TODO: Player name!
			m_OutputMs->WriteSize();
			int sent = m_Socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
			m_State = ClientState::HELLO_SENT;
		}
		break;
	case ClientState::PLAYING:
		{
			CMovement* lMovement = CEngine::GetInstance().GetMovement();
			m_OutputMs->Reset();
			uint8_t packetType = PacketType::PT_ReplicationData;
			((MemoryStream*)m_OutputMs)->Serialize(packetType, PACKET_BIT_SIZE);
			lMovement->Serialize(m_OutputMs);
			m_OutputMs->WriteSize();
			int sent = m_Socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
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
		m_InputMs->Reset(p.buffer, p.size);
		uint8_t packetType;
		((MemoryStream*)m_InputMs)->Serialize(packetType, PACKET_BIT_SIZE);
		if (packetType == PacketType::PT_ReplicationData && m_State == ClientState::PLAYING) {
			auto receivedGameObjects = lReplicationManager.ReceiveReplicatedObjects(m_InputMs);
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
	m_OutputMs->Reset();
	uint8_t packetType = PacketType::PT_Disconnect;
	((MemoryStream*)m_OutputMs)->Serialize(packetType, PACKET_BIT_SIZE);
	m_OutputMs->WriteSize();
	m_Socket->Send(m_OutputMs->GetBufferPtr(), m_OutputMs->GetByteLength());
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