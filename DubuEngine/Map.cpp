#include "Game.h"
#include "ScaledDraw.h"

void Map::GenerateRandom(int alg) {
	// Seed
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
		render_mode = 0;

		// Choose base biome and create a virtual zone
		Biome base = (Biome)(rand() % 3);
		Biome zone[MAP_SIZE_X][MAP_SIZE_Y];

		// Fill virtual zone with base biome
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				zone[x][y] = base;
			}
		}

		// Create biome zones
		// To try lets do 1 rectangle biome first
		Biome first_biome = Biome_Water;
		/*int biome_w = rand() % MAP_SIZE_X;
		int biome_h = rand() % MAP_SIZE_Y;
		int biome_x = rand() % MAP_SIZE_X - biome_w;
		int biome_y = rand() % MAP_SIZE_Y - biome_h;*/
		int biome_w = 10;
		int biome_h = 10;
		int biome_x = 5;
		int biome_y = 5;

		// Write this biome's zone
		/*for (int i = 0; i < biome_w; i++) {
			for (int j = 0; j < biome_h; j++) {
				zone[biome_x + i][biome_y + j] = first_biome;
			}
		}*/
		for (int x = biome_x; x < (biome_x + biome_w); x++) {
			for (int y = biome_y; y < (biome_y + biome_h); y++) {
				zone[x][y] = first_biome;
			}
		}

		// Sort out sprites for all biomes
		/*	This will loop trough each zone block from top to bottom,
			then slowly moving right. We will choose a sprite depending on
			the relationship of that block between other blocks around it.
		*/
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				int temp_sprite = GetTileSprite(zone[x][y], BLoc_C);
				if (y == 0) {
					// Top edge of map
					if (x == 0) {
						// Top left corner of map
						// We will not check the blocks above and on left

					}
				}
				tile[x][y] = temp_sprite;
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

int GetTileSprite(Biome biome, int location) {
	switch (biome) {
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