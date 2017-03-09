#pragma once

#include "CollisionBox.h"
#include "MapObject.h"
#include "TilesInfo.h"
#include "Butterfly.h"

#include <list>
#include <vector>

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

	// Sort objects and create blocks
	void OrganizeObjects();

	// Writes all the solid CollisionBoxes into a vector
	void CreateSolids();

	// Generates a random map based on seed
	void GenerateRandom(int alg);	

	
	/* ====================== SortPlayerObjects =========================
	 *	Sorts the map object list in a way so all players are in correct
	 *	locations. This function assumes that all current objects are already
	 *	sorted by their y coordinate (smallest to highest).
	 */
		void SortPlayerObjects(Game* g);


	/* ======================== RenderObjects ===========================
	 *	Renders all objects on map (fauna, players, etc.)
	 */
		void RenderObjects(Game* g, SpriteStruct* sprites);


	/* ========================== RenderGird ============================
	 *	Renders the grid (for debug only)
	 */
		void RenderGrid(Game* g, SpriteStruct* sprites);


	/* ========================= RenderTiles ============================
	 *	Renders the map's tiles
	 */
		void RenderTiles(Game* g, SpriteStruct* sprites);


	/* ======================== RenderBorders ===========================
	 *	Renders the map's borders
	 */
		void RenderBorders(Game* g, SpriteStruct* sprites);


	/* ====================== GenerateRandomShape =======================
	 *	Generates a random shape in a biome matrix within the square specified
	 */
		void GenerateRandomStamps(Biome zone[][MAP_SIZE_Y],
			Biome new_biome,					// Desired biome to generate
			int x, int y,						// Start x/y for square
			int w, int h);						// Width/Height for square


	/* ====================== SortSpritesFromZone =======================
	 *	Sorts all sprites in the map based on the zone specified
	 */
		void SortSpritesFromZone(Biome zone[][MAP_SIZE_Y]);

	/* ======================== ChangeRenderMode ========================
	 *	Changes render mode
	 *
	 *	@param newRenderMode new render mode for assign
	 *	@return false/true whether new render mode wasn't/was set
	 */
		bool ChangeRenderMode(int newRenderMode);

	/* ========================= GetRenderMode ==========================
	 *	Returns current render mode
	 */
		int GetRenderMode();

	/* ======================== ChangeForestMode ========================
	 *	Changes forest generation mode
	 *
	 *	@param newForestMode new forest mode for assign
	 *	@return false/true whether new forest mode wasn't/was set
	 */
		bool ChangeForestMode(int newForestMode);
		
	/* ========================== AddPeePuddle ==========================
	 *	Adds pee puddle on (x, y)
	 */
		void AddPeePuddle(int x, int y);

	/* ======================= RenderButterflies ========================
	 *	Renders the butterflies
	 */
		void RenderButterflies(Game* g, SpriteStruct* sprites);

	/* ====================== PopulateButterflies =======================
	 *	Spawns randomly-distributed butterflies
	 */
		void PopulateButterflies(int amount);

private:
	/* ===================== RenderObjectsOnBlockY ======================
	 *	Renders all objects corresponding to block y (fauna, players, etc.)
	 */
		void RenderObjectsOnBlockY(Game* g, SpriteStruct* sprites, int y, int xMin, int xMax, size_t& playerIndex);

	/* ==================== GenerateMapWithBaseBiome ====================
	 *	Generates a random map with a random base biome
	 */
		void GenerateMapWithBaseBiome();

	/* =========== GenerateRandomMapWithAppropriateNeighbours ===========
	 *	Generates a random map with appropiate neighbour tiles
	 */
		void GenerateRandomMapWithAppropriateNeighbours();

	/* ============================ TrimMap =============================
	 *	Trim map with water tiles to make an island
	 */
		void TrimMap();

	/* ========================= TrimMapBorder ==========================
	 *	Trim map border
	 *
	 *	@param xStart start cell x
	 *	@param yStart start cell y
	 *	@param xMoveDirection strip start move direction x
	 *	@param yMoveDirection strip start move direction y
	 *	@param xTrimDirection trim strip direction x
	 *	@param yTrimDirection trim strip direction y
	 */
		void TrimMapBorder(int xStart, int yStart, int xMoveDirection, int yMoveDirection, int xTrimDirection, int yTrimDirection);

	/* =========================== TrimStrip ============================
	 *	Trim strip at the map border
	 *
	 *	@param xStart start cell x
	 *	@param yStart start cell y
	 *	@param xTrimDirection trim strip direction x
	 *	@param yTrimDirection trim strip direction y
	 *	@param length strip length
	 */
		void TrimStrip(int xStart, int yStart, int xTrimDirection, int yTrimDirection, int length);
		
	/* ======================== AdjustWaterSides ========================
	 *	Adjusts water tile sides
	 *
	 *	@param x x-coordinate of tile patched
	 *	@param y y-coordinate of tile patched 
	 */
		void AdjustWaterSides(int x, int y);

	/* =========================== BuildRoads ===========================
	 *	Generates roads from already generated water bioms map
	 */
		void BuildRoads();

	/* =========================== BuildRoad ============================
	 *	Tries to build road between two junctions
	 *
	 *	@param a first junction
	 *	@param b second junction
	 *	@return false/true if road wasn't/was built
	 */
		bool BuildRoad(std::pair<int, int> a, std::pair<int, int> b);

	/* ======================= BuildStraightRoad ========================
	 *	Builds straight road between two junctions
	 *
	 *	@param a first junction
	 *	@param b second junction
	 *  @param actualBuild false for checking, true for building
	 *	@return false/true if road hasn't/has been built
	 */
		bool BuildStraightRoad(std::pair<int, int> a, std::pair<int, int> b, bool actualBuild);

	/* ========================== BFSMarkTiles ==========================
	 *	Applies breadth-first-search starting from tiles with dist = 0
	 *	Ends running on dist = maxDist
	 *	One of BFSInit should be executed before using this function to init dist = 0 tiles
	 *
	 *	@param maxDist maximal distance for bfs
	 */
		void BFSMarkTiles(int maxDist);

	/* ========================== BFSInitWater ==========================
	 *	Initializes map dist array for BFSMarkTiles
	 *	Marks starting tiles with 0
	 *	Marks non-starting tiles with _InfiniteDist
	 *	Starting tiles are water tiles
	 */
		void BFSInitWater();
		
	/* ======================== BFSInitWaterDirt ========================
	 *	Initializes map dist array for BFSMarkTiles
	 *	Marks starting tiles with 0
	 *	Marks non-starting tiles with _InfiniteDist
	 *	Starting tiles are water/dirt tiles
	 */
		void BFSInitWaterDirt();

	 /* ======================== PutRoadSegment =========================
	 *	Puts road tile with a thickness
	 *
	 *	@param x tile x
	 *	@param y tile y
	 *  @param horizontalThick true for horizontal thickness, false for vertical
	 */
		void PutRoadSegment(int x, int y, bool horizontalThick);

	/* ======================== AdjustDirtSides =========================
	 *	Puts reliable dirt tiles to adjust dirt sides
	 */
		void AdjustDirtSides();

	/* =================== AdjustDirtPlaceAdditional ====================
	 *	Puts additional dirt to adjust dirt sides
	 *
	 *	@param s starting tile of adjusting tiles area
	 */
		void AdjustDirtPlaceAdditional(std::pair<int, int> s);
		
	/* ========================= CanPatchSquare =========================
	 *	Check whether we can patch square 2x2 with tiles (x, y), (x + move.x), (x, y + move.y), (x + move.x, y + move.y)
	 */
		bool CanPatchSquare(int x, int y, std::pair<int, int> move);
		
	/* ====================== AdjustDirtPlacePatch ======================
	 *	Patches dirt tile to adjust dirt sides
	 *
	 *	@param x x-coordinate of tile patched
	 *	@param y y-coordinate of tile patched 
	 *	@return true if 2x2 dirt patch has been placed, false if tile turned into grass
	 */
		bool AdjustDirtPlacePatch(int x, int y);

	/* ===================== PopulateRandomObjects ======================
	 *	Generates random flowers, trees, bushes, etc.
	 */
		void PopulateRandomObjects();

	/* ========================= GenerateForest =========================
	 *	Generates forest bioms
	 */
		void GenerateForest();

	/* =================== GenerateForestWithDensity ====================
	 *	Generates forest biom with given density
	 *	All forest tiles should be put in _Queue before calling this function
	 *	Density is related to tiles, density = 1 means there will be approximately 1 tree per tile
	 *
	 *	@param density forest density
	 */
		void GenerateForestWithDensity(double density);
		
	/* ======================== ViewForestPlace =========================
	 *	Walks on free grass territory starting from (xs, ys)
	 *	All free tiles are in _Queue after this function
	 *
	 *	@param xs x for starting tile
	 *	@param ys y for starting tile
	 *	@return number of tiles in free space
	 */
		int ViewForestPlace(int xs, int ys);

	/* ======================== GenerateGreenery ========================
	 *	Generates additional greenery around the map
	 */
		void GenerateGreenery();
		
	/* ============================= InMap ==============================
	 *	Checks whether point is within map or not
	 */
		bool InMap(int x, int y);
		
	/* ======================== BuildTileBySides ========================
	 *	Builds simplified tile by 4 sides
	 */
		TilesInfo::Tile BuildTileBySides(int x, int y);
		
	/* ========================= BlockForObject =========================
	 *	Finds appropriate block for object
	 */
		inline std::pair<int, int> BlockForObject(const MapObjectInfo::MapObject& obj);
		
	/* ========================= BlockForPoint ==========================
	 *	Finds appropriate block for point on map
	 */
		inline std::pair<int, int> BlockForPoint(int x, int y);

public:
	static const int OBJECT_BLOCKS_CNT = 10;				// Blocks per axis
public:
	int tile[MAP_SIZE_X][MAP_SIZE_Y];						// tile[x][y]
	std::vector<Butterfly> Butterflies;						// Cosmetic butterflies
	std::vector<CollisionBox> solid;						// Solids (collisions/can't pass trough)
	std::vector<CollisionBox> except_solid;					// Special collision boxes to except solid collision
	std::vector<MapObjectInfo::MapObject> Objects;			// Map objects (fauna, etc.)
	std::vector<MapObjectInfo::MapObject*> ObjectBlock[OBJECT_BLOCKS_CNT][OBJECT_BLOCKS_CNT]; // Map objects in block
	std::vector<MapObjectInfo::MapObject*> GrassObjects;	// Map grass objects
	std::vector<MapObjectInfo::MapObject> Players;			// Players
	std::vector<MapObjectInfo::MapObject> Bones;			// Bones hidden on the map
	std::vector<MapObjectInfo::MapObject> PeePuddles;		// Pee puddles
	int seed;												// Seed for random generation
private:
	int _RenderMode = 0;
	int _RenderModeCnt = 1;
	int _ForestMode = 1;
	int _ForestModeCnt = 2;

private:
	int _Dist[MAP_SIZE_X][MAP_SIZE_Y];						// Distance to tile array
	std::vector<std::pair<int, int>> _Queue;				// Tile queue
	std::vector<std::pair<int, int>> _TemporaryQueue;		// Tile queue for temporary needs
	std::vector<int> _TemporaryVector;						// Vector for temporary needs

private:
	static const int _NeighbourWayCnt = 4;
	static const std::pair<int, int> _NeighbourWay[_NeighbourWayCnt];
	static const int _MinTrim = 1;
	static const int _MaxAdditionalTrim = 2;
	static const int _LakesToRoadsSpawnDist = 1;
	static const int _WaterDirtToForestSpawnDist = 2;
	static const int _JunctionChance = 70;
	static const int _RoadThickness = 1;
	static const int _MaxJunctionDistanceForRoad = 15;
	static const int _MinForestTiles = 10;
	static const int _MinPixelDistanceBetweenTrees = 30;
	static const int _FlowersCntMultiplier = 10;
	static const int _InfiniteDist = (MAP_SIZE_X + MAP_SIZE_Y + 5) * TILE_SIZE;
};

void UpdateMapAnimations(Game* g);
bool TileIsWater(int tile_id);
bool TileIsGrass(int tile_id);
bool TileIsDirt(int tile_id);
CollisionBox GetCollisionFromTile(int tile_id, int x, int y);