#include "../DubuEngineServer/DEProtocol.h"
#include "PacketHandler.h"
#include "GameHandler.h"
#include "Game.h"
#include "Multiplayer.h"
#include <sstream>

/* I use these macros in HandlePacket function
   Instead of accessing p trough a conversion to its derivative
   class, it's easier to use a macro */

#define p1b ((PacketBuffer1*)p)
#define p2b ((PacketBuffer2*)p)
#define ppi ((PacketPlayerInfo*)p)
#define pps ((PacketPlayerState*)p)
#define ppa ((PacketPlayerAction*)p)
#define ps ((PacketScore*)p)
#define pgi ((PacketGInfo*)p)

void HandlePacket(Game* g, Packet* p) {
	int deriv = p->deriv();
	int type = p->type();

	// Each deriv
	if (deriv == DEP_DERIV_PING) {
		switch (type) {
		case PACKET_TYPE_PING:
			AddPacketToQueue(g, new Packet(PACKET_TYPE_PING_ECHO));
			break;
		case PACKET_TYPE_PING_ECHO:
			g->ping = (int)((al_get_time() - g->time_sent_ping) * 1000);
			break;
		}
	} else if (deriv == DEP_DERIV_1BUFF) {
		switch (type) {
			case PACKET_TYPE_PLAYER_DISCONNECT:
				int pID = stoi(p1b->buffer_1);
				g->Players[pID].connected = false;
				g->score.score_info[pID].active = false;
				AddChatMessage(g->chat, "__SYSTEM__", 139, 49, 121, (g->Players[pID].name + " has has left the game.").c_str());
			break;
		}
	} else if (deriv == DEP_DERIV_2BUFF) {
		switch (type) {
			// Temp
		case PACKET_TYPE_CHAT_MESSAGE:
			AddChatMessage(g->chat, p2b->buffer_1, 255, 255, 255, p2b->buffer_2);
			break;
		case PACKET_TYPE_AUTHENTICATE:
			g->pl.name = g->logini.username_input;
			g->connected = true;

			// Send our info
			PacketPlayerInfo* pinfo = new PacketPlayerInfo(PACKET_TYPE_PLAYER_INFO);
			AddToBuffer(pinfo->name, g->pl.name);
			pinfo->w = g->pl.w;
			pinfo->h = g->pl.h;
			pinfo->sprite_id = g->pl.sprite_id;
			AddPacketToQueue(g, pinfo);
			break;
		}
	} else if (deriv == DEP_DERIV_PINFO) {
		// Update player info
		int player_id = ppi->p_id;
		g->Players[player_id].connected = true;
		g->Players[player_id].name = ppi->name;
		g->Players[player_id].sprite_id = ppi->sprite_id;
		g->Players[player_id].w = ppi->w;
		g->Players[player_id].h = ppi->h;
	} else if (deriv == DEP_DERIV_PSTATE) {
		// Update player state
		int player_id = pps->p_id;
		g->Players[player_id].connected = true;
		g->Players[player_id].x = pps->x;
		g->Players[player_id].y = pps->y;
		g->Players[player_id].sprite_frame = pps->frame;
		g->Players[player_id].facing = pps->facing;
	} else if (deriv == DEP_DERIV_PACTION) {
		// Update player actions
		int player_id = ppa->p_id;
		g->Players[player_id].digging = ppa->digging;
		g->Players[player_id].dig_timer = ppa->dig_timer;
		g->Players[player_id].peeing = ppa->peeing;
		g->Players[player_id].pee_timer = ppa->pee_timer;
		g->Players[player_id].sprinting = ppa->sprinting;
	} else if (deriv == DEP_DERIV_SCORE) {
		// Update score
		g->score.score_info[ps->p_id] = ps->score;
	} else if (deriv == DEP_DERIV_GINFO) {
		// Update game info
		g->game_mode = (GameMode)pgi->game_mode;
		g->game_duration.minutes = pgi->minutes;
		g->game_duration.seconds = pgi->seconds;
		g->game_duration.ticking = pgi->ticking;
		g->game_duration.inverted = pgi->inverted;
		g->map.seed = pgi->seed;
		g->map.SetTrim(pgi->trim);
		if(pgi->rebuild) LoadMultiplayer(g);
	} else if (deriv == DEP_DERIV_MAP) {
		// Update map structure
	}
}

void QueueActionsPacket(Game* g) {
	if (g->connected) {
		PacketPlayerAction* packet = new PacketPlayerAction(PACKET_TYPE_PLAYER_ACTION);

		// Digging
		packet->digging = g->pl.digging;
		packet->dig_timer = g->pl.dig_timer;

		// Peeing
		packet->peeing = g->pl.peeing;
		packet->pee_timer = g->pl.pee_timer;

		// Sprinting
		packet->sprinting = g->pl.sprinting;

		// Add packet to queue
		AddPacketToQueue(g, packet);
	}
}