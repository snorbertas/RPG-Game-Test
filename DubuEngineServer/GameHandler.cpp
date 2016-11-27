#include "DEProtocol.h"
#include "Game.h"
#include "GameHandler.h"

void HandleGame(Game* g) {
	for (int i=0; i < MAX_PLAYERS; i++) {
		if (g->SocketUsed[i]) {
			Packet* p = new Packet(PACKET_TYPE_PING);
			AddPacketToQueue(&g->players[i], p);
			g->players[i].ReadyToSendPackets = true;
		}
	}
}

int AddPacketToQueue(Player* pl, Packet* p) {
	for (int i = 0; i < MAX_PACKETS_SERVER; i++) {
		if (pl->PacketQueue[i].empty()) {
			pl->PacketQueue[i].hold(p);
			pl->ReadyToSendPackets = true;
			return i;
		}
	}

	// Queue is full
	cout << "Can't keep up with sending packets!\n";
	return -1;
}