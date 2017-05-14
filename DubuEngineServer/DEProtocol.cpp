#include "DEProtocol.h"
#include <string.h>
#include <sstream>
using namespace std;

void AddToBuffer(char* buffer, int data) {
	size_t len = strlen(buffer);
	if (len > 0) {
		strncpy(buffer + len, " ", 1);
		len++;
	}
	strncpy(buffer + len, to_string(data).c_str(), to_string(data).size());
}
void AddToBuffer(char* buffer, string& data) {
	size_t len = strlen(buffer);
	if (len > 0) {
		strncpy(buffer + len, " ", 1);
		len++;
	}
	strncpy(buffer + len, data.c_str(), data.size());
}
void AddToBuffer(char* buffer, const char* data) {
	size_t len = strlen(buffer);
	if (len > 0) {
		strncpy(buffer + len, " ", 1);
		len++;
	}
	strncpy(buffer + len, data, strlen(data));
}

int GetPacketSize(uint16_t deriv_id) {
	if (deriv_id == DEP_DERIV_PING) return sizeof(Packet);
	if (deriv_id == DEP_DERIV_1BUFF) return sizeof(PacketBuffer1);
	if (deriv_id == DEP_DERIV_2BUFF) return sizeof(PacketBuffer2);
	if (deriv_id == DEP_DERIV_PINFO) return sizeof(PacketPlayerInfo);
	if (deriv_id == DEP_DERIV_PSTATE) return sizeof(PacketPlayerState);
	if (deriv_id == DEP_DERIV_PACTION) return sizeof(PacketPlayerAction);
	if (deriv_id == DEP_DERIV_SCORE) return sizeof(PacketScore);
	if (deriv_id == DEP_DERIV_GINFO) return sizeof(PacketGInfo);
	if (deriv_id == DEP_DERIV_MAP) return sizeof(PacketMap);
	return 0;
}