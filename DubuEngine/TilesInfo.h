#pragma once

#include "Sprites.h"
#include <vector>

class TilesInfo {
private:
	TilesInfo() {}

public:
	enum ESubstance {GRASS = 0, DIRT, WATER, ESUBSSTANCE_SIZE};
	struct Tile {
		Tile() : SpriteId(0) {
			for (int i = 0; i < 9; ++i)
				Side[i] = GRASS;
		}
		ESubstance Side[9];
		int SpriteId;
	};
	class Biom {
	public:
		Biom(ESubstance innerSubstance, ESubstance outerSubstance) : InnerSubstance(innerSubstance), OuterSubstance(outerSubstance) {}
		void CreateTiles(Tile* tiles) const;

	public:
		ESubstance InnerSubstance;
		ESubstance OuterSubstance;
	};
	static const int BiomCnt = 3;
	static const int BiomTileCnt = 13;
	static const Biom Bioms[BiomCnt];
	static Tile BiomTiles[BiomCnt][BiomTileCnt];

private:
	static void AdjustNeighboursLeftRight(const Tile& l, const Tile& r);
	static void AdjustNeighboursUpDown(const Tile& u, const Tile& d);

public:
	static void AdjustTilesInfo();
	static int GetAppropriateTile(int leftTile, int upTile, int badTile = -1);

public:
	enum {TILE_NEIGHBOUR_LEFT = 0, TILE_NEIGHBOUR_RIGHT, TILE_NEIGHBOUR_UP, TILE_NEIGHBOUR_DOWN, TILE_NEIGHBOUR_SIZE};
	static std::vector<int> TileNeighbours[TILE_NEIGHBOUR_SIZE][MAX_TILE_SPRITES];
private:
	static std::vector<int> TempAppropriateTiles;
};