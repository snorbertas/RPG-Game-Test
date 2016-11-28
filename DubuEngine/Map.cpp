#include "Game.h"
#include "ScaledDraw.h"

enum Biome {Biome_Grass, Biome_Ground, Biome_Water};

void Map::GenerateRandom(int alg) {
	// Generation code
	srand(seed);

	switch (alg) {
	case 0:
		// Generation mode A (Literal random)
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				tile[x][y] = rand() % 41;
			}
		}
		break;
	case 1:
		// Generation mode B
		break;
	case 2:
		// Generation mode C
		// In this generation mode I will take a few steps
		/* =================================================*/
		/*	STEP 1: Choose default (global) biome
				This will be the base biome of our map.
				ex.: Water could represent an ocean or huge lake,
				thus the next biomes created are like "islands".

			STEP 2: Create the biome zones
				Biomes will have different shapes and sizes.

			STEP 3: Sort out the sprites.
				This will choose which sprites should be rendered to
				suit the biome blocks. It'll careful analyze edges and
				corners. */
		/* =================================================*/
		// This generation mode will use render mode 0 (Literal)

		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				tile[x][y] = GetSpriteTile(Biome_Grass, 5);
			}
		}
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

int GetSpriteTile(int biome_id, int location) {
	switch (biome_id) {
	case Biome_Grass:
		if (location >= 0 && location <= 4) {
			return location - 1;
		} else if (location == 5) {
			// There are 3 different center sprites for grass
			// Pick at random
			return (rand() % 3) + 4;
		} else if (location <= 9) {
			return location + 1;
		}
		break;
	case Biome_Ground:
		if (location >= 0 && location <= 9) {
			return location + 14;
		}
		break;
	case Biome_Water:
		if (location >= 0 && location <= 9) {
			return location + 27;
		}
		break;
	}

	// Todo: There are more "corner" sprites? Will figure out later
	// (I think they're for tiny 2x2 biomes, hmm, we'll see)
	return 0;
}