#pragma once

#ifndef H_PACKET
#define H_PACKET

#define PACKET_BIT_SIZE 3

enum PacketType : uint8_t {
	PT_Hello = 0,
	PT_ReplicationData,
	PT_ReplicationDeltas,
	PT_RPC,
	PT_Disconnect,
	PT_MAX
};

#endif