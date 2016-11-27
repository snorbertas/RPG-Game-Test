#include "../DubuEngineServer/DEProtocol.h"
#include "PacketHandler.h"
#include "GameHandler.h"
#include "Chat.h"

/* I use these macros in HandlePacket function
   Instead of accessing p trough a conversion to its derivative
   class, it's easier to use a macro */

#define p1b ((PacketBuffer1*)p)
#define p2b ((PacketBuffer2*)p)

void HandlePacket(Game* g, Packet* p) {
	int deriv = p->deriv();
	int type = p->type();

	// Each deriv
	if (deriv == DEP_DERIV_PING) {
		switch (type) {
		case PACKET_TYPE_PING_ECHO:
			g->ping = (int)((al_get_time() - g->time_sent_ping) * 1000);
			break;
		}
	} else if (deriv == DEP_DERIV_1BUFF) {
		switch (type) {
		}
	} else if (deriv == DEP_DERIV_2BUFF) {
		switch (type) {
			// Temp
		case PACKET_TYPE_CHAT_MESSAGE:
			AddChatMessage(g->chat, p2b->buffer_1, 255, 255, 255, p2b->buffer_2);
			break;
		case PACKET_TYPE_AUTHENTICATE:
			g->pl.name = g->logini.username_input;
			NewGame(g);
			g->connected = true;
			break;
		}
	}
}