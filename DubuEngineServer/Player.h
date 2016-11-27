#pragma once

#include "DEProtocol.h"

class Player
{
public:
	Player();
	int id = 0;								// Player ID
	PacketQ PacketQueue[MAX_PACKETS_SERVER];// Packet queue
	bool ReadyToSendPackets = false;		// Set to true when there's a packet ready to be sent by server
};