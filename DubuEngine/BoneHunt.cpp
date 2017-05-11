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
	int trim = 41 - (level * 1.33333);

	// Auto-setting
	setting = BoneHuntLevelSetting(5 + level, trim, 50 + (level * 10), 0, 0, 0, Node(50, 50));

	// Auto-player spawn
	while (!TileIsPathable(g->map.tile[setting.player_spawn.x][setting.player_spawn.y])) {
		int x = trim + (rand() % (Map::MAP_SIZE_X - (trim * 2)));
		int y = trim + (rand() % (Map::MAP_SIZE_Y - (trim * 2)));
		setting.player_spawn = Node(x, y);
	}

	// Auto-bone spawn
	int bones = 3 + level;

	// Before we spawn bones: decide medal standards
	setting.gold_standard = (bones * 75) + (setting.time_limit * 4);
	setting.silver_standard = (bones * 75) + (setting.time_limit * 2);
	setting.bronze_standard = bones * 75;

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

void CalculateBoneHuntScore(Game* g) {
	// Bones score (50 points per bone found)
	int score = g->score_singleplayer.bones * 50;

	// Territory score (1 point per square marked)
	g->score_singleplayer.territory = 0;
	for (int x = 0; x < Map::MAP_SIZE_X; ++x) {
		for (int y = 0; y < Map::MAP_SIZE_Y; ++y) {
			if (g->map.zone[x][y].Owner == -1) {
				++g->score_singleplayer.territory;
			}
		}
	}
	score += g->score_singleplayer.territory;

	// Time score (8 points per second left)
	score += ((g->game_duration.minutes * 60) + g->game_duration.seconds) * 8;

	// Update total score
	g->score_singleplayer.score = score;

	// Medal
	ProgressData::ProgressMedal medal = ProgressData::MEDAL_BRONZE;
	if (g->score_singleplayer.score >= g->lv_settings.gold_standard) {
		medal = ProgressData::MEDAL_GOLD;
	} else if (g->score_singleplayer.score >= g->lv_settings.silver_standard) {
		medal = ProgressData::MEDAL_SILVER;
	}
	if (medal > g->progress.BoneHunt.Level[g->level - 1].Medal) {
		g->progress.BoneHunt.Level[g->level - 1].Medal = medal;
	}

	// Complete level
	g->progress.BoneHunt.Level[g->level - 1].Complete = true;

}