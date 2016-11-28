#include "Game.h"
#include "ScaledDraw.h"

void Map::GenerateRandom(int alg) {
	// Generation code
	srand(seed);

	switch (alg) {
	case 0:
		// Generation mode 1 (Literal random)
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				tile[x][y] = rand() % 41;
			}
		}
		break;
	case 1:
		// Generation mode 2
		break;
	case 2:
		// Generation mode 3
		// In this generation mode I will take a few steps
		/*	STEP 1: Choose default (global) biome
				This will be the base biome of our map.
				ex.: Water could represent an ocean or huge lake,
				thus the next biomes created are like "islands".
			STEP 2: Create the biome zones
				It seems the tiles require a minimum of 3 sprites
				both vertically and horizontally to form a complete
				pattern. Thus the minimum biome size will be 3x3 big.
				Biomes will have different shapes and sizes, not just
				rectangles.

			STEP 3: Sort out the sprites.
				This will choose which sprites should be rendered to
				suit the biome blocks. It'll careful analyze edges and
				corners. */

		break;
	}
}

void Map::Render(Game* g, SpriteStruct* sprites) {
	auto img_tile = sprites->img_tile;
	switch (render_mode) {
	case 0:
		// Render mode 0 (Literal render)
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				DrawImage(g, img_tile[tile[x][y]],
					      (x * TILE_SIZE) + g->camera.x,
						  (y * TILE_SIZE) + g->camera.y, 0);
			}
		}
		break;
	case 1:
		// Render mode 1
		break;
	}
}