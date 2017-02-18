#pragma once

#include "DEProtocol.h"

class Player
{
public:
	Player();
	int id = 0;								// Player ID
	PacketQ PacketQueue[MAX_PACKETS_SERVER];// Packet queue
	bool ReadyToSendPackets = false;		// Set to true when there's a packet ready to be sent by server

	// Connection info
	int ping = 0;
	int time_sent_ping = 0;

	// Player info
	std::string name = "null";
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int sprite = 0;
};