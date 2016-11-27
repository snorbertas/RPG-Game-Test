#pragma once
#define MAX_PACKETS_SERVER 100
#define MAX_PACKETS_CLIENT 50
#define PACKET_TYPE_PING 2
#define PACKET_TYPE_PING_ECHO 3
#define PACKET_TYPE_AUTHENTICATE 100
#define PACKET_TYPE_SERVER_MESSAGE 101
#define PACKET_TYPE_CHAT_MESSAGE 102
#define PACKET_TYPE_PLAYER_STATE 103
#define PACKET_TYPE_PLAYER_NAME 104
#define PACKET_TYPE_HIT 105
#define PACKET_TYPE_ACTION 106
#include <iostream>
#include <string.h>
using namespace std;

// DubuEngine Packet Derivatives
#define DEP_EMPTY 0
#define DEP_DERIV_PING 1
#define DEP_DERIV_1BUFF 2
#define DEP_DERIV_2BUFF 3

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