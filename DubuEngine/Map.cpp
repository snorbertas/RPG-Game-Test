#include "Collision.h"
#include "Game.h"
#include "GameRenderer.h"
#include "ScaledDraw.h"

#include <algorithm>

const std::pair<int, int> Map::_NeighbourWay[_NeighbourWayCnt] = {make_pair(0, 1), make_pair(0, -1), make_pair(1, 0), make_pair(-1, 0)};

static bool pointInSegment(int start, int end, int p) {
	return start <= p && p <= end;
}

static bool intersectingSegments(int a1, int a2, int b1, int b2) {
	return pointInSegment(a1, a2, b1) || 
		   pointInSegment(a1, a2, b2) || 
		   pointInSegment(b1, b2, a1) || 
		   pointInSegment(b1, b2, a2);
}

static bool intersectingRectangles(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	return intersectingSegments(x1, x1 + w1, x2, x2 + w2) && 
		   intersectingSegments(y1, y1 + h1, y2, y2 + h2);
}

bool Map::InMap(int x, int y) {
	return x > -1 && y > -1 && x < MAP_SIZE_X && y < MAP_SIZE_Y;
}

static void PopulateRandomObjects(Map* map);

void Map::GenerateRandomMapWithAppropriateNeighbours() {
	tile[0][0] = 4;
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

	BuildRoads();

	for (int i = 0; i < MAP_SIZE_X; ++i) {
		for (int j = 0; j < MAP_SIZE_Y; ++j) {
			if (tile[i][j] == 4)
				tile[i][j] += rand() % 3;
		}
	}

	PopulateRandomObjects(this);
}

bool Map::BuildRoad(std::pair<int, int> a, std::pair<int, int> b) {
	if (a > b)
		swap(a, b);

	if (sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)) > _MaxJunctionDistanceForRoad)
		return false;

	_TemporaryQueue.clear();
	_TemporaryQueue.push_back(a);

	std::pair<int, int> s = a;
	int xCur = a.first;
	int yCur = a.second;
	int yAdd;
	if (a.second < b.second)
		yAdd = 1;
	else
		yAdd = -1;
	for (int xCur = a.first; xCur <= b.first; ++xCur) {
		int yLast = b.second;
		if (xCur != b.first) {
			double yDouble = s.second + (b.second - s.second) * ((xCur + 1 - s.first) / double(b.first - s.first + 1));
			yLast = static_cast<int>(yDouble + 1e-9 * yAdd);
		}

		bool nonWaterCells = true;
		int yMove = yCur - yAdd;
		do {
			yMove += yAdd;
			if (TilesInfo::GetSubstanceBySpriteId(tile[xCur][yMove]) == TilesInfo::WATER) {
				nonWaterCells = false;
				break;
			}
		} while (yMove != yLast);
		if (!nonWaterCells) {
			yMove -= yAdd;
			++xCur;
			for (; xCur <= b.first; ++xCur) {
				if (TilesInfo::GetSubstanceBySpriteId(tile[xCur][yMove]) == TilesInfo::WATER) {
					return false;
				}
				if (TilesInfo::GetSubstanceBySpriteId(tile[xCur][yMove + yAdd]) != TilesInfo::WATER) {
					s = std::make_pair(xCur, yMove + yAdd);
					yLast = s.second;
					_TemporaryQueue.push_back(s);
					break;
				}
			}
			if (xCur > b.first)
				return false;
		}

		yCur = yLast;
	}

	_TemporaryQueue.push_back(b);

	size_t size = _TemporaryQueue.size();
	if (size == 0)
		return false;
	bool canBuildStraight = true;
	for (size_t i = 0; i < size - 1; ++i) {
		if (!BuildStraightRoad(_TemporaryQueue[i], _TemporaryQueue[i + 1], false)) {
			canBuildStraight = false;
			break;
		}
	}
	if (!canBuildStraight)
		return false;
	for (size_t i = 0; i < size - 1; ++i) {
		BuildStraightRoad(_TemporaryQueue[i], _TemporaryQueue[i + 1], true);
	}
	return true;
}

bool Map::BuildStraightRoad(std::pair<int, int> a, std::pair<int, int> b, bool actualBuild) {
	int xCur = a.first;
	int yCur = a.second;
	int yAdd;
	if (a.second < b.second)
		yAdd = 1;
	else
		yAdd = -1;
	bool horizontalThick = (abs(a.first - b.first) < abs(a.second - b.second));
	for (int xCur = a.first; xCur <= b.first; ++xCur) {
		int yLast = b.second;
		if (xCur != b.first) {
			double yDouble = a.second + (b.second - a.second) * (double(xCur + 1 - a.first) / (b.first - a.first + 1));
			yLast = static_cast<int>(yDouble + 1e-9 * yAdd);
		}

		int yMove = yCur - yAdd;
		do {
			yMove += yAdd;
			if (!actualBuild) {
				if (TilesInfo::GetSubstanceBySpriteId(tile[xCur][yMove]) == TilesInfo::WATER)
					return false;
			} else {
				PutRoadSegment(xCur, yMove, horizontalThick);
			}
		} while (yMove != yLast);

		yCur = yLast;
	}
	return true;
}

void Map::BFSInitWater() {
	for (size_t i = 0; i < MAP_SIZE_X; ++i)
		for (size_t j = 0; j < MAP_SIZE_Y; ++j)
			_Dist[i][j] = _InfiniteDist;
	for (size_t i = 0; i < MAP_SIZE_X; ++i)
		for (size_t j = 0; j < MAP_SIZE_Y; ++j)
			if (TilesInfo::GetTileBySpriteId(tile[i][j]).GetSubstance() == TilesInfo::WATER)
				_Dist[i][j] = 0;
}

void Map::BFSMarkTiles(int maxDist) {	
	_Queue.clear();
	for (size_t i = 0; i < MAP_SIZE_X; ++i)
		for (size_t j = 0; j < MAP_SIZE_Y; ++j)
			if (_Dist[i][j] == 0)
				_Queue.push_back(std::make_pair(i, j));

	for (size_t i = 0; i < _Queue.size(); ++i) {
		int x = _Queue[i].first;
		int y = _Queue[i].second;
		int d = _Dist[x][y];
		if (d == _LakesToRoadsSpawnDist)
			break;

		for (size_t j = 0; j < _NeighbourWayCnt; ++j) {
			int xNew = x + _NeighbourWay[j].first;
			int yNew = y + _NeighbourWay[j].second;
			if (InMap(xNew, yNew) && _Dist[xNew][yNew] == _InfiniteDist) {
				_Dist[xNew][yNew] = d + 1;
				_Queue.push_back(std::make_pair(xNew, yNew));
			}
		}
	}
}

void Map::BuildRoads() {
	BFSInitWater();
	BFSMarkTiles(_LakesToRoadsSpawnDist);

	_Queue.clear();
	for (size_t i = 0; i < MAP_SIZE_X; ++i) {
		for (size_t j = 0; j < MAP_SIZE_Y; ++j) {
			if (TilesInfo::GetTileBySpriteId(tile[i][j]).GetSubstance() != TilesInfo::GRASS ||
				_Dist[i][j] != _InfiniteDist ||
				rand() % _JunctionChance != 0)
				continue;
			_Queue.push_back(std::make_pair(i, j));
			tile[i][j] = 19;
		}
	}

	size_t size = _Queue.size();
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = i + 1; j < size; ++j) {
			BuildRoad(_Queue[i], _Queue[j]);
		}
	}

	AdjustDirtSides();
}

void Map::PutRoadSegment(int x, int y, bool horizontalThick) {
	std::pair<int, int> move;
	if (horizontalThick)
		move = std::make_pair(1, 0);
	else
		move = std::make_pair(0, 1);
	int forwardSteps = (_RoadThickness + 1) / 2;
	int backwardSteps = _RoadThickness - forwardSteps;
	int xCur = x, yCur = y;
	for (int i = 0; i < forwardSteps; ++i, xCur += move.first, yCur += move.second) {
		if (!InMap(xCur, yCur) || TilesInfo::GetSubstanceBySpriteId(tile[xCur][yCur]) == TilesInfo::WATER)
			break;
		tile[xCur][yCur] = 19;
	}
	xCur = x - move.first, yCur = y - move.second;
	for (int i = 0; i < backwardSteps; ++i, xCur -= move.first, yCur -= move.second) {
		if (!InMap(xCur, yCur) || TilesInfo::GetSubstanceBySpriteId(tile[xCur][yCur]) == TilesInfo::WATER)
			break;
		tile[xCur][yCur] = 19;
	}
}

bool Map::CanPatchSquare(int xa, int ya, std::pair<int, int> move) {
	int xb = xa + move.first, yb = ya;
	int xc = xa, yc = ya + move.second;
	int xd = xa + move.first, yd = ya + move.second;
	if (!InMap(xa, ya) || !InMap(xb, yb) || !InMap(xc, yc) || !InMap(xd, yd))
		return false;
	if (TilesInfo::GetSubstanceBySpriteId(tile[xa][ya]) == TilesInfo::WATER || 
		TilesInfo::GetSubstanceBySpriteId(tile[xb][yb]) == TilesInfo::WATER || 
		TilesInfo::GetSubstanceBySpriteId(tile[xc][yc]) == TilesInfo::WATER || 
		TilesInfo::GetSubstanceBySpriteId(tile[xd][yd]) == TilesInfo::WATER)
		return false;
	return true;
}

bool Map::AdjustDirtPlacePatch(int x, int y) {
	for (size_t i = 0; i < _NeighbourWayCnt; ++i) {
		for (size_t j = i + 1; j < _NeighbourWayCnt; ++j) {
			std::pair<int, int> way = std::make_pair(_NeighbourWay[i].first + _NeighbourWay[j].first, 
													 _NeighbourWay[i].second + _NeighbourWay[j].second);
			if (way.first == 0)
				continue;

			if (CanPatchSquare(x, y, way)) {
				tile[x][y] = 19;
				tile[x + way.first][y] = 19;
				tile[x][y + way.second] = 19;
				tile[x + way.first][y + way.second] = 19;
				return true;
			}
		}
	}

	// if we can't patch with a square
	tile[x][y] = 4;
	return false;
}

TilesInfo::Tile Map::BuildTileBySides(int x, int y) {
	TilesInfo::Tile t;
	t.Side[4] = t.Side[1] = t.Side[3] = t.Side[5] = t.Side[7] = TilesInfo::GetSubstanceBySpriteId(tile[x][y]);

	if (y > 0)
		t.Side[1] = TilesInfo::GetSubstanceBySpriteId(tile[x][y - 1]);
	if (x > 0)
		t.Side[3] = TilesInfo::GetSubstanceBySpriteId(tile[x - 1][y]);
	if (x < MAP_SIZE_X - 1)
		t.Side[5] = TilesInfo::GetSubstanceBySpriteId(tile[x + 1][y]);
	if (y < MAP_SIZE_Y - 1)
		t.Side[7] = TilesInfo::GetSubstanceBySpriteId(tile[x][y + 1]);

	if (t.Side[1] == TilesInfo::WATER)
		t.Side[1] = TilesInfo::GRASS;
	if (t.Side[3] == TilesInfo::WATER)
		t.Side[3] = TilesInfo::GRASS;
	if (t.Side[5] == TilesInfo::WATER)
		t.Side[5] = TilesInfo::GRASS;
	if (t.Side[7] == TilesInfo::WATER)
		t.Side[7] = TilesInfo::GRASS;

	return t;
}

static bool CanAdjustTile(const TilesInfo::Tile& tile) {
	int grassSides = 0;
	if (tile.Side[1] == TilesInfo::GRASS)
		++grassSides;
	if (tile.Side[3] == TilesInfo::GRASS)
		++grassSides;
	if (tile.Side[5] == TilesInfo::GRASS)
		++grassSides;
	if (tile.Side[7] == TilesInfo::GRASS)
		++grassSides;
	return grassSides < 2 || (grassSides == 2 && tile.Side[1] != tile.Side[7]);
}

void Map::AdjustDirtPlaceAdditional(std::pair<int, int> s) {	
	if (AdjustDirtPlacePatch(s.first, s.second))
		return ;
	_Queue.clear();
	_Queue.push_back(s);
	
	for (size_t i = 0; i < _Queue.size(); ++i) {
		int x = _Queue[i].first;
		int y = _Queue[i].second;
		for (size_t j = 0; j < _NeighbourWayCnt; ++j) {
			int xNew = x + _NeighbourWay[j].first;
			int yNew = y + _NeighbourWay[j].second;
			if (InMap(xNew, yNew) && 
				TilesInfo::GetSubstanceBySpriteId(tile[xNew][yNew]) == TilesInfo::DIRT && 
				!CanAdjustTile(BuildTileBySides(xNew, yNew))) {
				if (!AdjustDirtPlacePatch(xNew, yNew))
					_Queue.push_back(std::make_pair(xNew, yNew));
			}
		}
	}
}

void Map::AdjustDirtSides() {
	for (int x = 0; x < MAP_SIZE_X; ++x) {
		for (int y = 0; y < MAP_SIZE_Y; ++y) {
			if (tile[x][y] != 19)
				continue;
			if (!CanAdjustTile(BuildTileBySides(x, y)))
				AdjustDirtPlaceAdditional(std::make_pair(x, y));
		}
	}

	for (int x = 0; x < MAP_SIZE_X; ++x) {
		for (int y = 0; y < MAP_SIZE_Y; ++y) {
			if (tile[x][y] != 19)
				continue;
			tile[x][y] = TilesInfo::GetSpriteIdByTile(BuildTileBySides(x, y));
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

// Temporary function to spawn and test object rendering/interaction
static void PopulateRandomObjects(Map* map) {
	// First reset the vector
	map->object.clear();
	map->bone.clear();

	// Then add random poop
	for (int i = 0; i < 500; i++) {
		int random_id = rand() % (MapObject::MapObjectTree_SO + 4);
		if (random_id > MapObject::MapObjectTree_SO) random_id += 6;
		map->object.push_back(MapObject(
			random_id,
			rand() % (map->MAP_SIZE_X * map->TILE_SIZE),
			rand() % (map->MAP_SIZE_Y * map->TILE_SIZE)));
	}

	// Sort by y
	map->object.sort([](MapObject& a, MapObject& b) {return a.y < b.y;});

	// Temp 50 bones
	for (int i = 0; i < 50; i++) {
		int rand_x = rand() % Map::MAP_SIZE_X * Map::TILE_SIZE;
		int rand_y = rand() % Map::MAP_SIZE_Y * Map::TILE_SIZE;
		MapObject rand_bone(MapObject::MapObjectBone, rand_x, rand_y);
		map->bone.push_back(rand_bone);
	}
}

static bool ObjectHasCollision(int object_id) {
	// Tress have collision
	if (object_id >= MapObject::MapObjectTree_BG &&
		object_id <= MapObject::MapObjectTree_SO) return true;
	return false;
}

static CollisionBox GetCollisionFromObject(MapObject map_object) {
	// Working with id
	auto id = map_object.id;

	// Initial x/y values
	int ini_x = map_object.x;
	int ini_y = map_object.y;
	int ini_w = 7;
	int ini_h = 7;

	// Create a CollisionBox to return and set it to the initial x/y position
	CollisionBox cb(ini_x, ini_y, ini_w, ini_h);

	// Re-size for specific objects
	if (id == MapObject::MapObjectTree_BG ||
		id == MapObject::MapObjectTree_BD ||
		id == MapObject::MapObjectTree_BO) {
		cb.x += 15;
		cb.y += 54;
		cb.w = 34;
		cb.h = 9;
	} else
	if (id == MapObject::MapObjectTree_SG ||
		id == MapObject::MapObjectTree_SD ||
		id == MapObject::MapObjectTree_SO) {
		cb.x += 17;
		cb.y += 56;
		cb.w = 28;
		cb.h = 7;
	}

	return cb;
}

static bool TileHasExceptBox(int tile_id) {
	// Water tiles have collision
	if (tile_id == 28 || tile_id == 30 ||
		tile_id == 34 || tile_id == 36) return true;
	return false;
}

static CollisionBox GetExceptBoxFromTile(int tile_id, int x, int y) {
	// Initial x/y values
	int ini_x = x * Map::TILE_SIZE;
	int ini_y = y * Map::TILE_SIZE;
	int ini_w = 7;
	int ini_h = 7;

	// Create a CollisionBox to return and set it to the initial x/y position
	CollisionBox cb(ini_x, ini_y, ini_w, ini_h);

	// Re-size for specific tiles
	if (tile_id == 28 || tile_id == 34) {
		cb.x += 7;
	} else if (tile_id == 30 || tile_id == 36) {
		cb.x += 50;
	}
	if (tile_id == 28 || tile_id == 30) {
		cb.y += 7;
	} else if (tile_id == 34 || tile_id == 36) {
		cb.y += 50;
	}

	return cb;
}

static bool TileHasCollision(int tile_id) {
	// Water tiles have collision
	if (tile_id >= 28 && tile_id <= 40) return true;
	return false;
}

static CollisionBox GetCollisionFromTile(int tile_id, int x, int y) {
	// Initial x/y values
	int ini_x = x * Map::TILE_SIZE;
	int ini_y = y * Map::TILE_SIZE;
	int ini_w = Map::TILE_SIZE;
	int ini_h = Map::TILE_SIZE;

	// Create a CollisionBox to return and set it to the initial x/y position
	CollisionBox cb(ini_x, ini_y, ini_w, ini_h);

	// Re-size for specific tiles
	if (tile_id == 28 || tile_id == 31 || tile_id == 34) {
		cb.x += 7;
		cb.w -= 7;
	} else if (tile_id == 30 || tile_id == 33 || tile_id == 36) {
		cb.w -= 7;
	}
	if (tile_id == 28 || tile_id == 29|| tile_id == 30) {
		cb.y += 7;
		cb.h -= 7;
	} else if (tile_id == 34 || tile_id == 35 || tile_id == 36) {
		cb.h -= 7;
	}

	return cb;
}

void Map::CreateSolids() {
	// First reset the vectors
	solid.clear();
	except_solid.clear();

	// First tiles
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_X; y++) {
			if (TileHasCollision(tile[x][y])) {
				solid.push_back(GetCollisionFromTile(tile[x][y], x, y));
			}
			if (TileHasExceptBox(tile[x][y])) {
				except_solid.push_back(GetExceptBoxFromTile(tile[x][y], x, y));
			}
		}
	}

	// Then objects
	for (std::list<MapObject>::iterator it = object.begin(); it != object.end(); it++) {
		if (ObjectHasCollision(it->id)) {
			solid.push_back(GetCollisionFromObject(*it));
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
		// Generation mode B
		GenerateRandomMapWithAppropriateNeighbours();
		break;
	case 2:
		// Generation mode C
		GenerateMapWithBaseBiome();
		PopulateRandomObjects(this);
		break;
	}

	// Create colission boxes for solids
	CreateSolids();
}

void Map::SortPlayerObjects(Game* g) {
	// Create a vector to store player pointers
	vector<Player*> players;

	// Local player
	players.push_back(&g->pl);

	// Multi players
	if (g->connected) {
		for (int i = 0; i < Game::MAX_PLAYERS; i++) {
			if (g->Players[i].connected) {
				players.push_back(&g->Players[i]);
			}
		}
	}

	// Sort them by their y value
	sort(players.begin(), players.end(), [](const Player* a, const Player* b) { return a->y < b->y; });

	// Boolean to store which players are already inserted
	bool* inserted_player = new bool[players.size()];
	for (int i = 0; i < players.size(); i++) { inserted_player[i] = false; }

	// First remove all player objects
	object.remove_if([](MapObject& a) { return (a.id >= MapObject::MapObjectPlayer_L); });

	// Loop trough each object to check where we can insert player objects
	for (std::list<MapObject>::iterator it = object.begin(); it != object.end(); it++) {
		for (int i = 0; i < players.size(); i++) {
			if (!inserted_player[i]) {
				if (it->y + it->h > players[i]->y + players[i]->h) {
					// Insert player object
					MapObject player_object = MapObject(
						MapObject::MapObjectPlayer_M_0 + players[i]->pID,
						players[i]->x, players[i]->y);
					object.insert(it, player_object);
					inserted_player[i] = true;
				}
			}
		}
	}

	// The rest of player objects are on bottom of all objects
	// Todo: maybe reverse order? can't test since there's a weird bug
	for (int i = 0; i < players.size(); i++) {
		if (!inserted_player[i]) {
			// Insert player object
			MapObject player_object = MapObject(
				MapObject::MapObjectPlayer_M_0 + players[i]->pID,
				players[i]->x, players[i]->y);
			object.push_front(player_object);
		}
	}

	// Release dynamic memory
	delete[] inserted_player;
}

void Map::RenderObjects(Game* g, SpriteStruct* sprites) {
	auto img_object = sprites->img_object;
	for (std::list<MapObject>::iterator it = object.begin(); it != object.end(); it++) {
		if (it->id == MapObject::MapObjectGrass_0 ||
			it->id == MapObject::MapObjectGrass_1) {
			// Tall Grass
			DrawImage(g,
				img_object[GetObjectSprite(*it)],
				it->x + g->camera.x - 18,
				it->y + g->camera.y - 36, 0);
		} else if (it->id < MapObject::MapObjectPlayer_L) {
			// Rendering immobile objects
			DrawImage(g,
				img_object[GetObjectSprite(*it)],
				it->x + g->camera.x,
				it->y + g->camera.y, 0);

			// If it's a tree
			if (it->id >= MapObject::MapObjectTree_BG &&
				it->id <= MapObject::MapObjectTree_SO) {

				// Draw the top of tree
				DrawImage(g,
					img_object[GetObjectSprite(*it, 1)],
					it->x + g->camera.x,
					it->y - (it->h) + g->camera.y, 0);
			}
		} else if (it->id == MapObject::MapObjectPlayer_L) {
			// Rendering Players/NPCs
			RenderPlayer(g, g->pl, sprites);
		} else if (it->id >= MapObject::MapObjectPlayer_M_0) {
			RenderPlayer(g, g->Players[it->id - MapObject::MapObjectPlayer_M_0], sprites);
		}
	}
}

void Map::RenderGrid(Game* g, SpriteStruct* sprites) {
	// Temp (Draw grid and collision boxes for solids)

	if (g->debug.grid) {
		for (int i = 0; i < solid.size(); i++) {
			DrawRectangle(g,
				solid[i].x + g->camera.x, solid[i].y + g->camera.y,
				solid[i].w, solid[i].h, 255, 0, 0, 0.2);
		}
		for (int i = 0; i < except_solid.size(); i++) {
			DrawRectangle(g,
				except_solid[i].x + g->camera.x, except_solid[i].y + g->camera.y,
				except_solid[i].w, except_solid[i].h, 0, 0, 255, 0.2);
		}
		for (int x = 0; x < MAP_SIZE_X; x++) {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				DrawOutline(g,
					x * TILE_SIZE + g->camera.x,
					y * TILE_SIZE + g->camera.y,
					TILE_SIZE, TILE_SIZE, 0, 50, 50, 1, 50);
			}
		}
	}
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

void UpdateMapAnimations(Game* g) {
	for (std::list<MapObject>::iterator it = g->map.object.begin(); it != g->map.object.end(); it++) {
		if (it->id == MapObject::MapObjectGrass_1) {
			// Decrement timer
			it->anim_timer--;

			// Revert to idle grass if timer ran out
			if (it->anim_timer == 0) {
				it->anim_timer = -1;
				it->id = MapObject::MapObjectGrass_0;
			}
		}
	}
}