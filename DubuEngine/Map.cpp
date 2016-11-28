#include "Game.h"
#include "ScaledDraw.h"
#include "TilesInfo.h"

void Map::GenerateRandomMapWithAppropriateNeighbours() {
	tile[0][0] = rand() % MAX_TILE_SPRITES;
	for (int i = 1; i < MAP_SIZE_X; ++i) {
		tile[i][0] = TilesInfo::GetAppropriateTile(tile[i - 1][0], -1);
	}
	for (int j = 1; j < MAP_SIZE_Y; ++j) {
		tile[0][j] = TilesInfo::GetAppropriateTile(-1, tile[0][j - 1]);
		bool badTileFlag = false;
		for (int i = 1; i < MAP_SIZE_X; ++i) {
			tile[i][j] = TilesInfo::GetAppropriateTile(tile[i - 1][j], tile[i][j - 1]);
			if (tile[i][j] == -1) {
				tile[i][j] = 4;
				if (!badTileFlag) {
					badTileFlag = true;
					tile[i - 1][j] = TilesInfo::GetAppropriateTile(tile[i - 2][j], tile[i - 1][j - 1], tile[i - 1][j]);
					if (tile[i - 1][j] == -1) {
						tile[i - 1][j] = 4;
						return ;
					} else {
						--i;
					}
				} else {
					return ;
				}
			} else {
				badTileFlag = false;
			}
		}
	}
}

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
		GenerateRandomMapWithAppropriateNeighbours();
		break;
	case 2:
		// Generation mode C (I think I will put this in separate functions lmao)
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
		Biome base = (Biome)(rand() % 3 + 1);
		Biome zone[MAP_SIZE_X][MAP_SIZE_Y];

		// Fill virtual zone with base biome
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				zone[x][y] = base;
			}
		}

		// Create biome zones
		// To try lets do 1 rectangle biome first
		Biome first_biome = Biome_Grass;
		/*int biome_w = rand() % MAP_SIZE_X;
		int biome_h = rand() % MAP_SIZE_Y;
		int biome_x = rand() % MAP_SIZE_X - biome_w;
		int biome_y = rand() % MAP_SIZE_Y - biome_h;*/
		int biome_w = 1;
		int biome_h = 1;
		int biome_x = 5;
		int biome_y = 5;

		for (int x = biome_x; x < (biome_x + biome_w); x++) {
			for (int y = biome_y; y < (biome_y + biome_h); y++) {
				zone[x][y] = first_biome;
			}
		}

		// Sort out sprites for all biomes (will make separate function)
		/*	This will loop trough each zone block from top to bottom,
			then slowly moving right. We will choose a sprite depending on
			the relationship of that block between other blocks around it.
		*/

		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				BlockLocation loc = BLoc_C;
				bool CommonTop = false;
				bool CommonBottom = false;
				bool CommonLeft = false;
				bool CommonRight = false;
				// Fill Corners
				bool CommonTopLeft = false;
				bool CommonTopRight = false;
				bool CommonBottomLeft = false;
				bool CommonBottomRight = false;

				// Check common blocks
				if (y != 0) {
					if(IsCommonTo(zone[x][y], zone[x][y - 1]))
						CommonTop = true;
				} else {
					CommonTop = true;
				}
				if (y < MAP_SIZE_Y) {
					if (IsCommonTo(zone[x][y], zone[x][y + 1]))
						CommonBottom = true;
				} else {
					CommonBottom = true;
				}
				if (x != 0) {
					if (IsCommonTo(zone[x][y], zone[x - 1][y]))
						CommonLeft = true;
				} else {
					CommonLeft = true;
				}
				if (x < MAP_SIZE_X) {
					if (IsCommonTo(zone[x][y], zone[x + 1][y]))
						CommonRight = true;
				} else {
					CommonRight = true;
				}

				// Corner fills
				if (x != 0 && y != 0) {
					if (IsCommonTo(zone[x][y], zone[x - 1][y - 1]))
						CommonTopLeft = true;
				} else {
					CommonTopLeft = true;
				}
				if (x < MAP_SIZE_X && y != 0) {
					if (IsCommonTo(zone[x][y], zone[x + 1][y - 1]))
						CommonTopRight = true;
				} else {
					CommonTopRight = true;
				}
				if (x != 0 && y < MAP_SIZE_Y) {
					if (IsCommonTo(zone[x][y], zone[x - 1][y + 1]))
						CommonBottomLeft = true;
				} else {
					CommonBottomLeft = true;
				}
				if (x < MAP_SIZE_X && y < MAP_SIZE_Y) {
					if (IsCommonTo(zone[x][y], zone[x + 1][y + 1]))
						CommonBottomRight = true;
				} else {
					CommonBottomRight = true;
				}

				// Choose the block location
				if (!CommonTopLeft && CommonTop && CommonLeft) {
						loc = BCor_TL;
				} else if (!CommonTopRight && CommonTop && CommonRight) {
						loc = BCor_TR;
				} else if (!CommonBottomLeft && CommonBottom && CommonLeft) {
						loc = BCor_BL;
				} else if (!CommonBottomRight && CommonBottom && CommonRight) {
						loc = BCor_BR;
				} else if (!CommonTop) {
					loc = BLoc_T;
					if (!CommonLeft)
						loc = BLoc_TL;
					if (!CommonRight)
						loc = BLoc_TR;
				} else if (!CommonBottom) {
					loc = BLoc_B;
					if (!CommonLeft)
						loc = BLoc_BL;
					if (!CommonRight)
						loc = BLoc_BR;
				} else {
					if (!CommonLeft)
						loc = BLoc_L;
					if (!CommonRight)
						loc = BLoc_R;
				}

				// Update the sprite id for that tile
				tile[x][y] = GetTileSprite(zone[x][y], loc);
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

int GetTileSprite(Biome biome, BlockLocation location) {
	switch (biome) {
	case Biome_Grass:
		if (location >= BLoc_TL && location <= BLoc_L) {
			return location - BLoc_TL;
		} else if (location == BLoc_C) {
			// There are 3 different center sprites for grass
			// Pick at random
			return (rand() % 3) + 4;
		} else if (location <= BCor_TL) {
			return location + 1;
		}
		break;
	case Biome_Ground:
		if (location >= BLoc_TL && location <= BCor_TL) {
			return location + 14;
		}
		break;
	case Biome_Water:
		if (location >= BLoc_TL && location <= BCor_TL) {
			return location + 27;
		}
		break;
	}
	return 0;
}

bool IsCommonTo(Biome center, Biome edge) {
	if (center == edge) return true;
	switch (center) {
	case Biome_Grass:
		if (edge == Biome_Ground) return true;
		if (edge == Biome_Water) return true;
		break;
	}
	return false;
}