#pragma once

#ifndef H_PACKET
#define H_PACKET

enum PacketType {
	PT_Hello,
	PT_ReplicationData,
	PT_RPC,
	PT_Disconnect,
	PT_MAX
};

#endif