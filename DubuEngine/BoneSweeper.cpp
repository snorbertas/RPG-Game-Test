#include "Game.h"
#include "BoneSweeper.h"

void SpawnRandomMines(Game* g, int mines, int x, int y, int w, int h) {
	for (int i = 0; i < mines; ++i) {
		int spawn_x = x + rand() % w;
		int spawn_y = y + rand() % h;
		g->map.zone[spawn_x][spawn_y].BoneSweeperReal = Zone::BoneSweeper::Mine;
	}
}

void CalculateRealBoneSweeper(Game* g) {
	// Loop trough every square
	for (int x = 0; x < Map::MAP_SIZE_X; ++x) {
		for (int y = 0; y < Map::MAP_SIZE_Y; ++y) {

			// Make sure its not a mine
			if (g->map.zone[x][y].BoneSweeperReal != Zone::BoneSweeper::Mine) {

				// Count the mines
				int mines = 0;

				// Loop trough 9x9 grid around this square
				for (int tx = x - 1; tx < x + 2; ++tx) {
					for (int ty = y - 1; ty < y + 2; ++ty) {

						// Check if the target is legal
						if (tx > 0 && tx < Map::MAP_SIZE_X && ty > 0 && ty < Map::MAP_SIZE_Y) {
							if (g->map.zone[tx][ty].BoneSweeperReal == Zone::BoneSweeper::Mine) {

								// It's a mine!
								mines++;
							}
						}
					}
				}

				// Mark it
				g->map.zone[x][y].BoneSweeperReal = (Zone::BoneSweeper)mines;
			}
		}
	}
}