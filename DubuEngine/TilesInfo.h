#pragma once

#include "Sprites.h"
#include <vector>

class TilesInfo {
private:
	TilesInfo() {}

public:
	enum ESubstance {GRASS = 0, DIRT, WATER, ESUBSSTANCE_SIZE};
	struct Tile {			// Side: 0 1 2
		ESubstance Side[9]; //		 3 4 5
	};						//		 6 7 8
	class Biom {
	public:
		Biom(ESubstance innerSubstance, ESubstance outerSubstance) : InnerSubstance(innerSubstance), OuterSubstance(outerSubstance) {}
		ESubstance InnerSubstance;
		ESubstance OuterSubstance;
		void ColourBiomTiles(int startTile) {
		}
	};
	static const Biom Bioms[3];
	static Tile Tiles[39];

public:
	static int GetBiomStart(int biom);
	static bool InBiom(int biom, int tileIndex);
	static int MoveTileIndexToBiom(int biom, int tileIndex);

private:
	static void AdjustTilesInfo(int biom, int outerBiom);
	static void AdjustTilesInfo(int biom, int outerBiom, int forwardDir, int backwardDir, const std::vector<std::pair<int, int>>& neighbourPairs);
public:
	static void AdjustTilesInfo();
	static bool AppropriateNeighbours(int tile, int neighbourTile, int direction);
	static int GetAppropriateTile(int leftTile, int upTile, int badTile = -1);

public:
	static const int BiomSize = 13;
	enum {TILE_NEIGHBOUR_LEFT = 0, TILE_NEIGHBOUR_RIGHT, TILE_NEIGHBOUR_UP, TILE_NEIGHBOUR_DOWN, TILE_NEIGHBOUR_SIZE};
	static std::vector<int> TileNeighbours[TILE_NEIGHBOUR_SIZE][MAX_TILE_SPRITES];
private:
private:
	static std::vector<int> TempAppropriateTiles;
};