#pragma once
#include <utility>

class Game;

static const std::pair<int, int> BoneHuntSeedAndTrim(int level) {
	// Seed, Trim
	if (level == 1) return std::make_pair(6, 40);
	return std::make_pair(0, 46);
}