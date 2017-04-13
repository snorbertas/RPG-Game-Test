#include "Game.h"
#include "BoneHunt.h"

void HandleBoneHunt(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	if (g->game_duration.minutes + g->game_duration.seconds == 0) {
		if (!g->weather.cycle.night) {
			// Start night
			g->weather.StartNight(g);
		} else {
			if (g->npc.size() == 0 && g->weather.cycle.timer <= 0) {
				// Spawn wolves
				// I want static spawns for wolves depending on each level so that has to wait for now
				SpawnNPC(g, NPC::Type::Monster, 3200, 3200);
			}
		}
	}
}