#pragma once
#include <utility>
#include <allegro5/allegro_audio.h>

class Game;

void HandleBoneHunt(Game* g, ALLEGRO_SAMPLE** sample_sfx);

static const std::pair<int, int> BoneHuntSeedAndTrim(int level) {
	// Seed, Trim
	if (level == 1) return std::make_pair(6, 40);
	return std::make_pair(0, 46);
}