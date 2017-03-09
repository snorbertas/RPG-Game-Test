#pragma once
#define MAX_PACKETS_SERVER 100
#define MAX_PACKETS_CLIENT 50
#define PACKET_TYPE_PING 2
#define PACKET_TYPE_PING_ECHO 3
#define PACKET_TYPE_AUTHENTICATE 100
#define PACKET_TYPE_SERVER_MESSAGE 101
#define PACKET_TYPE_CHAT_MESSAGE 102
#define PACKET_TYPE_PLAYER_INFO 103
#define PACKET_TYPE_PLAYER_STATE 104
#define PACKET_TYPE_PLAYER_ACTION 105
#define PACKET_TYPE_SCORE 106
#define PACKET_TYPE_PLAYER_DISCONNECT 107

#include <iostream>
#include <string.h>
#include "Score.h"
using namespace std;

// DubuEngine Packet Derivatives
#define DEP_EMPTY 0
#define DEP_DERIV_PING 1
#define DEP_DERIV_1BUFF 2
#define DEP_DERIV_2BUFF 3
#define DEP_DERIV_PINFO 4
#define DEP_DERIV_PSTATE 5
#define DEP_DERIV_PACTION 6
#define DEP_DERIV_SCORE 7

// The base packet class, contains no information except the type and deriv
class Packet {
public:
	Packet() : _Type(DEP_EMPTY), _Deriv(DEP_EMPTY) {}
	Packet(uint16_t type, uint16_t deriv = DEP_DERIV_PING) {
		_Type = type;
		_Deriv = deriv;
	}
	virtual ~Packet() {}
	uint16_t type() {
		return _Type;
	}
	uint16_t deriv() {
		return _Deriv;
	}
private:
	uint16_t _Type;
	uint16_t _Deriv;
};

/* Packet derivative with 1 buffer.
   The size should be optimized for each project specifically.
   Feel free to create packet derivatives to suit your needs. */
class PacketBuffer1 : public Packet {
public:
	PacketBuffer1(uint16_t type) : Packet(type, DEP_DERIV_1BUFF) {}
	char buffer_1[50] = "";
};

// Packet derivative with 2 buffers
class PacketBuffer2 : public Packet {
public:
	PacketBuffer2(uint16_t type) : Packet(type, DEP_DERIV_2BUFF) {}
	char buffer_1[50] = "";
	char buffer_2[50] = "";
};

// Packet derivative for player info
class PacketPlayerInfo : public Packet {
public:
	PacketPlayerInfo(uint16_t type) : Packet(type, DEP_DERIV_PINFO) {}
	// Player ID
	uint8_t p_id = 0;

	// Width/Height
	uint8_t w = 0;
	uint8_t h = 0;

	// Name
	char name[50] = "";

	// Visual
	uint16_t sprite_id = 0;
};

// Packet derivative for player state
class PacketPlayerState : public Packet {
public:
	PacketPlayerState(uint16_t type) : Packet(type, DEP_DERIV_PSTATE) {}
	// Player ID
	uint8_t p_id = 0;

	// Player x/y coordinates
	uint16_t x = 0;
	uint16_t y = 0;

	// Visual
	uint8_t frame = 0;
	uint8_t facing = 0;
};

// Packet derivative for player actions update
class PacketPlayerAction : public Packet {
public:
	PacketPlayerAction(uint16_t type) : Packet(type, DEP_DERIV_PACTION) {}
	// Player ID
	uint8_t p_id = 0;

	// States
	bool digging = false;
	int dig_timer = 0;
	bool peeing = false;
	int pee_timer = 0;
	bool sprinting = false;
};

// Packet derivative for score update
class PacketScore : public Packet {
public:
	PacketScore(uint16_t type) : Packet(type, DEP_DERIV_SCORE) {}
	// Player ID
	uint8_t p_id = 0;

	// Score info
	ScoreInfo score;
};

int GetPacketSize(uint16_t deriv_id);

/* Class for packet queue
   Each PacketQ stores 1 packet, use an array of PacketQ to make a queue */
class PacketQ {
public:
	bool empty() {
		return _Empty;
	}
	Packet* p() {
		if (_P == NULL) {
			cout << "Warning! Nullpointer returned\n";
		}
		return _P;
	}
	void free() {
		delete _P;
		_Empty = true;
		_P = NULL;
	}
	void hold(Packet* packet) {
		_P = packet;
		_Empty = false;
	}
private:
	bool _Empty = true;
	Packet* _P = NULL;
};

void AddToBuffer(char* buffer, int data);
void AddToBuffer(char* buffer, string& data);
void AddToBuffer(char* buffer, const char* data);