#pragma once

class Game;

struct SniffInfo {
	int closest_bone = -1;
	double closest_distance = -1;
};

void CalculateBoneDistances(Game* g);
void Sniff(Game* g);