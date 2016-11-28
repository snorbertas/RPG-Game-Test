#pragma once

// Biomes
enum Biome {
	Biome_None,
	Biome_Grass,
	Biome_Ground,
	Biome_Water
};

/* Block locations
Loc	=	Location
Cor	=	Corner/Fill
ERR	=	Error
T	=	Top
B	=	Bottom
L	=	Left
R	=	Right */
enum BlockLocation {
	BLoc_ERR,
	BLoc_TL,
	BLoc_T,
	BLoc_TR,
	BLoc_L,
	BLoc_C,
	BLoc_R,
	BLoc_BL,
	BLoc_B,
	BLoc_BR,
	BCor_BR,
	BCor_BL,
	BCor_TR,
	BCor_TL
};

/* =========================== GetSpriteTile ===========================
*		Returns the sprite ID for a desired tile.
*			Locations:
*			[1]	[2]	[3]
*			[4]	[5]	[6]
*			[7]	[8]	[9]
*		Example: If you want the top-right corner sprite for grass
*		GetSpriteID(Biome_Grass, BLoc_TR); (will return sprite ID 2)
*/
	int GetTileSprite(Biome biome, BlockLocation location);


/* ============================ IsCommonTo =============================
*		Returns true if the center sprite of first biome can succesfuly
*		merge with an edge of the second biome.
*		Otherwise returns false.
*/
	bool IsCommonTo(Biome center, Biome edge);

// Map Class
class Map {
public:
	// Constants
	static const int MAP_SIZE_X = 100;
	static const int MAP_SIZE_Y = 100;
	static const int TILE_SIZE = 64;

	// Constructor
	Map(int s) : seed(s) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				tile[x][y] = GetTileSprite(Biome_Grass, BLoc_C);
			}
		}

	}

	void GenerateRandom(int alg);		// Generates a random map based on seed
	void Render(Game* g, SpriteStruct* sprites);

	void GenerateRandomMapWithAppropriateNeighbours();

	/* =================== GenerateMapWithBaseBiome =====================
	 * Generates a random map with a random base biome
	 */
		void GenerateMapWithBaseBiome();


	/* ====================== GenerateRandomShape =======================
	 * Generates a random shape in a biome matrix within the square specified
	 */
		void GenerateRandomShape(Biome zone[][MAP_SIZE_Y],
			Biome new_biome,					// Desired biome to generate
			int x, int y,						// Start x/y for square
			int w, int h,						// Width/Height for square
			Biome touch_only = Biome_None);		// Optional: Only write on this biome type


	/* ====================== SortSpritesFromZone =======================
	 * Sorts all sprites in the map based on the zone specified
	 */
		void SortSpritesFromZone(Biome zone[][MAP_SIZE_Y]);


	// Variables
	int tile[MAP_SIZE_X][MAP_SIZE_Y];	// tile[x][y]
	int seed;							// Seed for random generation
	int render_mode = 0;
};