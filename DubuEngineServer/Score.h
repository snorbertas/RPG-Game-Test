#pragma once

class ScoreInfo {
public:
	bool active = false;
	int icon_id = 0;
	char name[50] = "";
	int ping = 0;
	int bones = 0;
	int territory = 0;
	int score = 0;

	void Reset() {
		active = false;
		name[0] = '\0';
		ping = 0;
		bones = 0;
		territory = 0;
		score = 0;
	}
};

class ScoreBoard {
public:
	bool show_score = false;
	ScoreInfo score_info[20];
};