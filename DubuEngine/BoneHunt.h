#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include <allegro5/allegro_audio.h>
#include "Pathing.h"

class Game;

void HandleBoneHunt(Game* g, ALLEGRO_SAMPLE** sample_sfx);

class BoneHuntLevelSetting {
public:
	// Spawns
	Node player_spawn = Node(50, 50);
	std::vector<Node> monster_spawn;
	std::vector<Node> bone_spawn;

	// Time limit (in seconds)
	int time_limit = 60;

	// Map settings
	int seed = 0;
	int trim = 46;

	// Medals (WIP still)
	int gold_standard = 3;
	int silver_standard = 2;
	int bronze_standard = 1;

	// Constructors
	// Default
	BoneHuntLevelSetting() {}

	// Full
	BoneHuntLevelSetting(int Seed, int Trim,
		int TimeLimit,
		int Gold, int Silver, int Bronze,
		Node PlayerSpawn) : seed(Seed), trim(Trim),
		time_limit(TimeLimit),
		gold_standard(Gold), silver_standard(Silver), bronze_standard(Bronze),
		player_spawn(PlayerSpawn) {}

	// Functions
	void AddBone(int x, int y) { bone_spawn.push_back(Node(x, y)); }
	void AddMonster(int x, int y) { monster_spawn.push_back(Node(x, y)); }
};

BoneHuntLevelSetting GetBoneHuntLevelSetting(Game* g, int level);