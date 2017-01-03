#pragma once
#include <vector>
#include <list>
#include "MapObject.h"
#include "CollisionBox.h"

// Biomes
enum Biome {
	Biome_None,
	Biome_Grass,
	Biome_Ground,
	Biome_Water
};

// Block locations
/*
	Key:
	Loc	=	Location
	Cor	=	Corner/Fill
	ERR	=	Error
	T	=	Top
	B	=	Bottom
	L	=	Left
	R	=	Right
*/
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

	// Writes all the solid CollisionBoxes into a vector
	void CreateSolids();

	// Generates a random map based on seed
	void GenerateRandom(int alg);	

	
	/* ====================== SortPlayerObjects =========================
	 * Sorts the map object list in a way so all players are in correct
	 * locations. This function assumes that all current objects are already
	 * sorted by their y coordinate (smallest to highest).
	 */
		void SortPlayerObjects(Game* g);


	/* ======================== RenderObjects ===========================
	 * Renders all objects on map (fauna, players, etc.)
	 */
		void RenderObjects(Game* g, SpriteStruct* sprites);


	/* ========================== RenderGird ============================
	 * Renders the grid (for debug only)
	 */
		void RenderGrid(Game* g, SpriteStruct* sprites);


	/* ========================= RenderTiles ============================
	 * Renders the map's tiles
	 */
		void RenderTiles(Game* g, SpriteStruct* sprites);


	/* ========= GenerateRandomMapWithAppropriateNeighbours =============
	 * Generates a random map with appropiate neighbouring tiles
	 */
		void GenerateRandomMapWithAppropriateNeighbours();


	/* =================== GenerateMapWithBaseBiome =====================
	 * Generates a random map with a random base biome
	 */
		void GenerateMapWithBaseBiome();


	/* ====================== GenerateRandomShape =======================
	 * Generates a random shape in a biome matrix within the square specified
	 */
		void GenerateRandomStamps(Biome zone[][MAP_SIZE_Y],
			Biome new_biome,					// Desired biome to generate
			int x, int y,						// Start x/y for square
			int w, int h);						// Width/Height for square


	/* ====================== SortSpritesFromZone =======================
	 * Sorts all sprites in the map based on the zone specified
	 */
		void SortSpritesFromZone(Biome zone[][MAP_SIZE_Y]);

private:
	/* ===================== GenerateBiomsRoadsMap ======================
	 *	Generates map based on idea of blocks of bioms and system of roads
	 */
		void GenerateBiomsRoadsMap();

		void GenerateWaterBioms(int minWaterTiles);
		int GenerateWaterBiom(int xs, int ys, int w, int h);
		void AddNeighbour(int x, int y);
		void AppendNeighbourToBiom(int index, int xs, int xf, int ys, int yf);
		void AdjustTileSides(int xs, int xf, int ys, int yf);

	/* ========================== Build Roads ===========================
	 *	Generates roads from already generated water bioms map
	 */
		void BuildRoads();

	/* ========================== Build Roads ===========================
	 *	Generates roads from already generated water bioms map
	 */
		/*void BuildRoad(std::pair<int, int> a, std::pair<int, int> b);*/

		
	/* ========================== Build Roads ===========================
	 *	Generates roads from already generated water bioms map
	 */
		bool InMap(int x, int y);

	// Variables
public:
	int tile[MAP_SIZE_X][MAP_SIZE_Y];	  // tile[x][y]
	list<MapObject> object;				  // Map objects (fauna, etc.)
	vector<CollisionBox> solid;			  // Solids (collisions/can't pass trough)
	vector<CollisionBox> except_solid;	  // Special collision boxes to except solid collision
	vector<MapObject> bone;				  // Bones hidden on the map
	int seed;							  // Seed for random generation
	int render_mode = 0;

private:
	bool _Visited[MAP_SIZE_X][MAP_SIZE_Y];		  // Tile visiting array
	int _Dist[MAP_SIZE_X][MAP_SIZE_Y];			  // Distance to tile array
	vector<std::pair<int, int>> _Queue;			  // Tile queue
	std::vector<std::pair<int, int>> _Neighbours; // Tile neighbours array to exisiting biom tiles

	// Constants
private:
	static const double _MaxWaterBiomSideToMapProportion;
	static const double _MaxBiomTilesOverhead;
	static const int _NeighbourWayCnt = 4;
	static const std::pair<int, int> _NeighbourWay[_NeighbourWayCnt];
	static const int _LakesToRoadsSpawnDist = 3;
	static const int _RoadChance = 50;
};