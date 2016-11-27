#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Game.h"
#include "GameHandler.h"

void NewGame(Game* g){
	g->scene = 1;
	g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
	g->Interfaces[INTERFACE_LOGIN].visible = false;
	g->Interfaces[INTERFACE_CHAT].visible = true;
	g->logini.logging_in = false;
}

void HandleGame(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Game handling code here
	HandleGamePackets(g);
}

void HandleGamePackets(Game* g) {
	// Temporary client code
	if (g->connected) {
		// Ping
		g->ticks_since_last_ping++;
		if (g->ticks_since_last_ping >= TPS) {
			g->ticks_since_last_ping = 0;
			// Increase ping by a second so player knows they're lagging
			if (g->ticks_since_last_ping - al_get_time() > g->ping * 2) {
				g->ping += 1000;
			}
			// Send a ping
			Packet* ping = new Packet(PACKET_TYPE_PING);
			AddPacketToQueue(g, ping);
		}
	}
}

void HandleGameInput(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	//
}

int SecondsToTicks(float seconds) {
	return (int)(seconds * TPS);
}

bool collide(int o1x, int o1y, int o1w, int o1h, int o2x, int o2y, int o2w, int o2h) {
	if (o1x <= o2x + o2w &&
		o1x + o1w >= o2x &&
		o1y <= o2y + o2h &&
		o1h + o1y >= o2y) {
		return true;
	}
	return false;
}

bool collide(int o1x, int o1y, int o1r, int o2x, int o2y, int o2r) {
	int dx = o1x - o2x;
	int dy = o1y - o2y;
	int distance = sqrt(dx * dx + dy * dy);

	if (distance < o1r + o2r) {
		return true;
	}
	return false;
}

void ShowMessage(Game* g, const char* msg1, const char* msg2) {
	g->Message1 = msg1;
	g->Message2 = msg2;
	g->Interfaces[INTERFACE_MESSAGE].visible = true;
}

void Fade(Game* g, int oi) {
	if (oi == 0) g->fade_out = SecondsToTicks(2.1);
	if (oi == 1) g->fade_in = SecondsToTicks(2.1);
}

bool MouseIsOn(Game* g, int x, int y, int w, int h) {
	if (collide(x, y, w, h, g->pos_x, g->pos_y, 0, 0)) {
		return true;
	}
	return false;
}

int AddPacketToQueue(Game* g, Packet* p) {
	for (int i = 0; i < MAX_PACKETS_CLIENT; i++) {
		if (g->PacketQueue[i].empty()) {
			g->PacketQueue[i].hold(p);
			g->ReadyToSendPackets = true;
			return i;
		}
	}

	// Queue is full
	cout << "Can't keep up with sending packets!\n";
	return -1;
}