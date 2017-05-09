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

BoneHuntLevelSetting GetBoneHuntLevelSetting(Game* g, int level) {
	// Create a setting
	BoneHuntLevelSetting setting;

	// Auto-trim
	int trim = 40 - (level * 1.33333);

	// Auto-setting
	setting = BoneHuntLevelSetting(5 + level, trim, 50 + (level * 10), 0, 0, 0, Node(50, 50));

	// Auto-spawn (todo)
	setting.player_spawn = Node(50, 50);

	// Auto-bone spawn (temp)
	int bones = 3 + level;
	srand(0);
	while (bones > 0) {
		int x = trim + (rand() % (Map::MAP_SIZE_X - (trim * 2)));
		int y = trim + (rand() % (Map::MAP_SIZE_Y - (trim * 2)));
		if (TileIsPathable(g->map.tile[x][y])) {
			if (std::find(setting.bone_spawn.begin(), setting.bone_spawn.end(), Node(x, y)) == setting.bone_spawn.end()) {
				setting.AddBone(x, y);
				--bones;
			}
		}
	}

	// Return setting
	return setting;
}