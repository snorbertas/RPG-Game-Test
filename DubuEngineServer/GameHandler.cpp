#include "DEProtocol.h"
#include "Game.h"
#include "GameHandler.h"

void HandleGame(Game* g) {
	for (int i=0; i < MAX_PLAYERS; i++) {
		if (g->SocketUsed[i]) {
			Packet* p = new Packet(PACKET_TYPE_PING);
			AddPacketToQueue(&g->players[i], p);
		}
	}

	// Send a score update
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (g->SocketUsed[i]) {
			// Loop trough all scores
			for (int i2 = 0; i2 < MAX_PLAYERS; i2++) {
				if(g->score.score_info[i2].active) {
					PacketScore* p = new PacketScore(PACKET_TYPE_SCORE);
					p->p_id = i2;
					p->score = g->score.score_info[i2];
					AddPacketToQueue(&g->players[i], p);
				}
			}
		}
	}

	//
	
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