#pragma once

#ifndef H_NETWORK_MANAGER_CLIENT
#define H_NETWORK_MANAGER_CLIENT

#include "Common\NetworkManager.h"
#include "Serializer\PacketStream.h"

#define SEGMENT_SIZE 1500

class CNetworkManagerClient : public NetworkManager
{
public:
	CNetworkManagerClient();
	virtual ~CNetworkManagerClient();
	virtual bool Init(uint16_t aPort) override;
	bool Connect();
	void UpdateSendingSockets(float aDeltaTime);
	void UpdateReceivingSockets(float aDeltaTime);
	void UpdatePackets(float aDeltaTime);
	void ManageDisconnection();
	void RenderImGui();
	float GetAverageTimeBetweenPackets() {
		return m_AverageTimeBetweenPackets;
	}
private:
	PacketStream m_PacketStream;
	virtual bool InitReflection() override;
	void AddSample(float sample);
	float m_AverageTimeBetweenPackets;
	uint32_t m_NumberOfSamples;
	float m_Timer;
};

#endif