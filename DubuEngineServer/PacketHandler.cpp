#include <iostream>
#include "DEProtocol.h"
#include "PacketHandler.h"
#include "GameHandler.h"
using namespace std;

/* I use these macros in HandlePacket function
Instead of accessing p trough a conversion to its derivative
class, it's easier to use a macro */

#define p1b ((PacketBuffer1*)p)
#define p2b ((PacketBuffer2*)p)
#define ppi ((PacketPlayerInfo*)p)
#define pps ((PacketPlayerState*)p)

void HandlePacket(Game* g, int pID, Packet* p) {
	int deriv = p->deriv();
	int type = p->type();

	// Each deriv
	if (deriv == DEP_DERIV_PING) {
		switch (type) {
		case PACKET_TYPE_PING:
			AddPacketToQueue(&g->players[pID], new Packet(PACKET_TYPE_PING_ECHO));
			break;
		}
	} else if (deriv == DEP_DERIV_1BUFF) {
	} else if (deriv == DEP_DERIV_2BUFF) {
		switch (type) {
		case PACKET_TYPE_AUTHENTICATE:
		{
			cout << "Login attempt: username: " << p2b->buffer_1 << ", pass: " << p2b->buffer_2 << endl;
			PacketBuffer2* test = new PacketBuffer2(PACKET_TYPE_AUTHENTICATE);
			AddToBuffer(test->buffer_1, "Hey! I heard you :)");
			AddToBuffer(test->buffer_2, "Congratulations, working 100%!");
			AddPacketToQueue(&g->players[pID], test);
			break;
		}
		case PACKET_TYPE_CHAT_MESSAGE:
			for (int i = 0; i < MAX_PLAYERS; i++) {
				if (g->SocketUsed[i]) {
					PacketBuffer2* temp = new PacketBuffer2(PACKET_TYPE_CHAT_MESSAGE);
					AddToBuffer(temp->buffer_1, p2b->buffer_1);
					AddToBuffer(temp->buffer_2, p2b->buffer_2);
					AddPacketToQueue(&g->players[i], temp);
				}
			}
			break;
		}
	} else if (deriv == DEP_DERIV_PINFO) {
		// Store info
		g->players[pID].name = ppi->name;
		g->players[pID].w = ppi->w;
		g->players[pID].h = ppi->h;
		g->players[pID].sprite = ppi->sprite_id;
	} else if (deriv == DEP_DERIV_PSTATE) {
		// Store state
		g->players[pID].x = pps->x;
		g->players[pID].y = pps->y;

		// Share state with all clients
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (g->SocketUsed[i] && i != pID) {
				PacketPlayerState* pstate = new PacketPlayerState(PACKET_TYPE_PLAYER_STATE);
				pstate->x = pps->x;
				pstate->y = pps->y;
				pstate->frame = pps->frame;
				pstate->facing = pps->facing;
				pstate->p_id = i;
				AddPacketToQueue(&g->players[i], pstate);
			}
		}
	}
}