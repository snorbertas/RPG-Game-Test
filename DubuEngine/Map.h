#pragma once

#define TILE_DEFAULT 4

class Map {
public:
	Map(int s) : seed(s) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				tile[x][y] = TILE_DEFAULT;
			}
		}

	}
	void GenerateRandom(int alg);		// Generates a random map based on seed
	void Render(Game* g, SpriteStruct* sprites);

	// Constants
	static const int MAP_SIZE_X = 100;
	static const int MAP_SIZE_Y = 100;
	static const int TILE_SIZE = 64;

	// Variables
	int tile[MAP_SIZE_X][MAP_SIZE_Y];	// tile[x][y]
	int seed;							// Seed for random generation
	int render_mode = 0;
};
