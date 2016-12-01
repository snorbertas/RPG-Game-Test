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
		int badTile = -1;
		for (int i = 1; i < MAP_SIZE_X; ++i) {
			if (i != 0)
				tile[i][j] = TilesInfo::GetAppropriateTile(tile[i - 1][j], tile[i][j - 1], badTile);
			else
				tile[i][j] = TilesInfo::GetAppropriateTile(-1, tile[i][j - 1], badTile);
			if (tile[i][j] == -1) {
				tile[i][j] = 4;
				badTile = tile[i - 1][j];
				i -= 2;
			} else {
				badTile = -1;
			}
		}
	}
}

void Map::GenerateMapWithBaseBiome() {
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
	Biome base = Biome_Grass;
	Biome zone[MAP_SIZE_X][MAP_SIZE_Y];

	// Fill virtual zone with base biome
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			zone[x][y] = base;
		}
	}

	/* 
		ATTEMPT TO MAKE RANDOM SHAPES
	*/
	GenerateRandomStamps(zone, Biome_Ground, 0, 0, 100, 100);

	// Sort out sprites for all biomes
	SortSpritesFromZone(zone);
}

void Map::GenerateRandomStamps(Biome zone[][MAP_SIZE_Y], Biome new_biome, int x, int y, int w, int h) {
	// x, y are the start position
	// w, h are width and height of the rectangle to generate the shape within

	// Random cross stamps
	// Can only generate on grass
	int stamps_left = sqrt(w * h) * 12;

	struct Point {
		Point() {}
		Point(int X, int Y) : x(X), y(Y) {}
		int x;
		int y;
	};

	// Matrix of allowed/banned points
	bool allowed_point[MAP_SIZE_X][MAP_SIZE_Y];
	for (int iX = 0; iX < MAP_SIZE_X; iX++) {
		for (int iY = 0; iY < MAP_SIZE_Y; iY++) {
			allowed_point[iX][iY] = true;
		}
	}

	while (stamps_left > 0) {
		// Choose random allowed point
		bool chosen_point = false;
		int pX, pY;
		while (!chosen_point) {
			pX = x + rand() % (w - 3);
			pY = y + rand() % (h - 3);
			if (allowed_point[pX][pY] == true) {
				chosen_point = true;
			}
		}

		// Draw a cross stamp at that starting point
		// Top
		zone[pX + 1][pY] = new_biome;
		zone[pX + 2][pY] = new_biome;

		// Middle
		for (int i = 0; i < 4; i++) {
			zone[pX + i][pY + 1] = new_biome;
			zone[pX + i][pY + 2] = new_biome;
		}

		// Bottom
		zone[pX + 1][pY + 3] = new_biome;
		zone[pX + 2][pY + 3] = new_biome;

		// Ban specific points
		/* There are 12 main points that should be banned relative
		   to this stamp */
		/*
			X = Our point
			B = Bans
			[ ] [ ] [ ] [B] [ ] [B] [ ] [ ] [ ]
			[ ] [B] [ ] [ ] [ ] [ ] [ ] [B] [ ]
			[ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
			[B] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [B]
			[ ] [ ] [ ] [ ] [X] [ ] [ ] [ ] [ ]
			[B] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [B]
			[ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
			[ ] [B] [ ] [ ] [ ] [ ] [ ] [B] [ ]
			[ ] [ ] [ ] [B] [ ] [B] [ ] [ ] [ ]
		*/
		if (pX - 3 > x && pY - 3 > y) {
			allowed_point[pX - 3][pY - 3] = false;
		}
		if (pX - 3 > x && pY + 3 < y + h) {
			allowed_point[pX - 3][pY + 3] = false;
		}
		if (pX + 3 < x + w && pY - 3 > y) {
			allowed_point[pX + 3][pY - 3] = false;
		}
		if (pX + 3 < x + w && pY + 3 < y + w) {
			allowed_point[pX + 3][pY + 3] = false;
		}
		if (pX - 1 > x && pY - 4 > y) {
			allowed_point[pX - 1][pY - 4] = false;
		}
		if (pX + 1 < x + w && pY - 4 > y) {
			allowed_point[pX + 1][pY - 4] = false;
		}
		if (pX + 4 < x + w && pY - 1 > y) {
			allowed_point[pX + 4][pY - 1] = false;
		}
		if (pX + 4 < x + w && pY + 1 < y + w) {
			allowed_point[pX + 4][pY + 1] = false;
		}
		if (pX - 1 > x && pY + 4 < y + h) {
			allowed_point[pX - 1][pY + 4] = false;
		}
		if (pX + 1 < x + w && pY + 4 < y + w) {
			allowed_point[pX + 1][pY + 4] = false;
		}
		if (pX - 4 > x && pY - 1 > y) {
			allowed_point[pX - 4][pY - 1] = false;
		}
		if (pX - 4 > x && pY + 1 < y + h) {
			allowed_point[pX - 4][pY + 1] = false;
		}

		// Decrement stamp count
		stamps_left--;
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
		// Generation mode B
		GenerateRandomMapWithAppropriateNeighbours();
		break;
	case 2:
		// Generation mode C
		GenerateMapWithBaseBiome();
		break;
	}
}

void Map::RenderBackgroundObjects(Game* g, SpriteStruct* sprites) {

}

void Map::RenderForegroundObjects(Game* g, SpriteStruct* sprites) {

}

void Map::RenderTiles(Game* g, SpriteStruct* sprites) {
	auto img_tile = sprites->img_tile;
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			DrawImage(g, img_tile[tile[x][y]],
				(x * TILE_SIZE) + g->camera.x,
				(y * TILE_SIZE) + g->camera.y, 0);
		}
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

void Map::SortSpritesFromZone(Biome zone[][MAP_SIZE_Y]) {
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
}