#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Game.h"
#include "GameHandler.h"
#include "Collision.h"
#include "Digging.h"
#include "Peeing.h"

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
	UpdateMapAnimations(g);
	HandleGameDuration(g);
	HandleMultiplayerMisc(g);
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

void HandleGameDuration(Game* g) {
	if (g->game_duration.ticking) {
		// Increment tick
		g->game_duration.ticks++;

		// Convert ticks into raw seconds
		g->game_duration.seconds = TicksToSeconds(g->game_duration.ticks);
		g->game_duration.minutes = 0;

		// Invert if counting down
		if (g->game_duration.inverted) {
			g->game_duration.seconds = g->game_duration.seconds_start - g->game_duration.seconds;
			if (g->game_duration.seconds < 0) g->game_duration.seconds = 0;
		}


		// Give 1 minutes for each 60 seconds
		while (g->game_duration.seconds > 59) {
			g->game_duration.minutes++;
			g->game_duration.seconds -= 60;
		}
	}
}

void HandleMultiplayerMisc(Game* g) {
	// Handle multi-player timers, animations, etc.
	if (g->connected) {
		for (int i = 0; i < g->MAX_PLAYERS; i++) {
			// Digging timer and animation
			HandleDigging(g, &g->Players[i]);

			// Peeing timer and animation
			HandlePeeing(g, &g->Players[i]);
		}
	}
}

void HandleGameInput(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	//
}

float TicksToSeconds(int ticks) {
	return (float)(ticks / TPS);
}

int SecondsToTicks(float seconds) {
	return (int)(seconds * TPS);
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