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
#include "BoneHunt.h"
#include "Interfaces.h"

void NewGame(Game* g){
	g->scene = 1;
	g->Interfaces[INTERFACE_MAIN_MENU].Hide();
	g->Interfaces[INTERFACE_LOGIN].Hide();
	g->Interfaces[INTERFACE_CHAT].Show();
	g->logini.logging_in = false;
	g->game_over = false;
	g->level_complete = false;
}

void HandleGame(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Game handling code here
	HandleGamePackets(g);
	UpdateMapAnimations(g);
	HandleGameDuration(g);
	HandleMultiplayerMisc(g);
	HandleButterflies(g);
	HandleDirtParticles(g);
	HandleGameOver(g);
	if (g->game_mode == GameMode::GM_BoneHunt) HandleBoneHunt(g, sample_sfx);
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

void HandleButterflies(Game* g) {
	for (size_t i = 0; i < g->map.Butterflies.size(); i++) {
		g->map.Butterflies[i].Tick();
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
	g->Interfaces[INTERFACE_MESSAGE].Show();
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
	delete(p);
	// Queue is full
	cout << "Can't keep up with sending packets!\n";
	return -1;
}

void GameOver(Game* g) {
	g->game_over = true;
	g->mouse_pathing = false;
	g->keys.left = false;
	g->keys.right = false;
	g->keys.up = false;
	g->keys.down = false;
	g->keys.sprint = false;
	HideAllInterfaces(g, INTERFACE_GAME_OVER);
	if (!g->level_complete) {
		g->Interfaces[INTERFACE_GAME_OVER].Show();
		g->Buttons[31].Hide();
		g->game_over_timer = SecondsToTicks(1.0);
	} else {
		g->Interfaces[INTERFACE_LEVEL_COMPLETE].Show();
	}

	// Explosion
	if (g->game_mode == GameMode::GM_BoneSweeper && !g->level_complete) {
		// Kill grass
		int count = 0;
		for (int x = g->pl.MapX() - 2; x < g->pl.MapX() + 3; ++x) {
			for (int y = g->pl.MapY() - 2; y < g->pl.MapY() + 3; ++y) {
				// Check if index is valid and correct shape
				if (x > 0 && x < Map::MAP_SIZE_X &&
					y > 0 && y < Map::MAP_SIZE_Y &&
					count != 0 && count != 4 && count != 20 && count != 25) {
					// If not water, replace with dirt
					if (!TileIsWater(g->map.tile[x][y])) {
						// TODO: Pick correct dirt sprite according to neigbhours
						g->map.tile[x][y] = 19;
					}
				}
				count++;
			}
		}
	}
}

void HandleGameOver(Game* g) {
	if (g->game_over_timer > 0) g->game_over_timer--;

	if (g->game_over_timer == 0) {
		// Enable buttons
		g->Buttons[31].Show();
	}
}