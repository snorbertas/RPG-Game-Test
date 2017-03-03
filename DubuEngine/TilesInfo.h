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
		bool operator == (const Tile& t) {
			for (size_t i = 0; i < 9; ++i)
				if (Side[i] != t.Side[i])
					return false;
			return true;
		}
		ESubstance GetSubstance() const {
			return Side[4];
		}
		ESubstance Side[9];
		int SpriteId;
	};
	class Biom {
	public:
		Biom(ESubstance innerSubstance, ESubstance outerSubstance) : 
			InnerSubstance(innerSubstance), 
			OuterSubstance(outerSubstance) {}
		void CreateTiles(std::vector<Tile>& tiles) const;

	public:
		ESubstance InnerSubstance;
		ESubstance OuterSubstance;
	};
public:
	static const size_t BIOM_CNT = 3;
	static const Biom Bioms[BIOM_CNT];
	static std::vector<Tile> BiomTiles[BIOM_CNT];

public:
	static void AdjustTilesInfo();
	static int GetAppropriateTile(int leftTile, int upTile, int badTile = -1);
	static int GetSpriteIdByTile(const Tile& tileWithoutSprite);
	static const Tile& GetTileBySpriteId(int spriteId);
	static ESubstance GetSubstanceBySpriteId(int spriteId);
private:
	static void AdjustNeighboursLeftRight(const Tile& l, const Tile& r);
	static void AdjustNeighboursUpDown(const Tile& u, const Tile& d);

public:
	enum {TILE_NEIGHBOUR_LEFT = 0, TILE_NEIGHBOUR_RIGHT, TILE_NEIGHBOUR_UP, TILE_NEIGHBOUR_DOWN, TILE_NEIGHBOUR_SIZE};
	static std::vector<int> TileNeighbours[TILE_NEIGHBOUR_SIZE][MAX_TILE_SPRITES];
private:
	static std::vector<int> TempAppropriateTiles;
};