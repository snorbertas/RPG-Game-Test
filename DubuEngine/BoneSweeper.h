#pragma once

class Game;

void SpawnRandomMines(Game* g,
	int mines = 20,
	int x = 0, int y = 0,
	int w = 100, int h = 100);

void CalculateRealBoneSweeper(Game* g);

void RenderKnownBoneSweeperInfo(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font);

void DigBoneSweeper(Game* g, int tx, int ty);