#pragma once

class ProgressData {
public:
	// Constants
	static const int BONE_HUNT_LEVELS = 30;

	// Medal types
	enum ProgressMedal { MEDAL_NONE, MEDAL_BRONZE, MEDAL_SILVER, MEDAL_GOLD };

	// Bone hunt saved structure
	struct ProgressBoneHunt {
		struct LevelBoneHunt {
			bool Complete = false;
			ProgressMedal Medal = MEDAL_NONE;
			int Score = 0;
		};
		LevelBoneHunt Level[BONE_HUNT_LEVELS];

		int CompleteCount() {
			for (int i = 0; i < BONE_HUNT_LEVELS ; i++) {
				if (!Level[i].Complete) return i + 1;
			}
			if (Level[BONE_HUNT_LEVELS - 1].Complete) return BONE_HUNT_LEVELS - 1;
			return 0;
		}
	};

	// Bone hunt progress
	ProgressBoneHunt BoneHunt;

	// Complete bonehunt level
	void CompleteBoneHuntLevel(int level, int score, ProgressMedal medal) {
		if (level <= BONE_HUNT_LEVELS) {
			// Mark as complete
			BoneHunt.Level[level - 1].Complete = true;

			// Update score and medal (if improved)
			if (score > BoneHunt.Level[level - 1].Score)	BoneHunt.Level[level - 1].Score = score;
			if (medal > BoneHunt.Level[level - 1].Medal)	BoneHunt.Level[level - 1].Medal = medal;
		}
	}

	// Save data
	void Save(const char* file_path);

	// Load data
	void Load(const char* file_path);
};