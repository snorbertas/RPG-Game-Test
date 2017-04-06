#pragma once

#include "Sprites.h"

#include <vector>

class TilesInfo {
private:
	TilesInfo() {}

public:
	enum ESubstance {GRASS = 0, DIRT, WATER_LAKE, WATER_SEA, ESUBSSTANCE_SIZE};
	class Tile {
	public:
		Tile() : SpriteId(0) {
			for (int i = 0; i < 9; ++i)
				Side[i] = GRASS;
		}

	public:
		bool operator== (const Tile& t) {
			for (size_t i = 0; i < 9; ++i)
				if (Side[i] != t.Side[i])
					return false;
			return true;
		}

	private:
		bool HasSubstance(ESubstance substance) const {
			for (size_t i = 0; i < 9; ++i)
				if (Side[i] == substance)
					return true;
			return false;
		}
	public:
		ESubstance GetMainSubstance() const {
			return Side[4];
		}
		bool HasGrass() const {
			return HasSubstance(GRASS);
		}
		bool HasDirt() const {
			return HasSubstance(DIRT);
		}
		bool HasWater() const {
			return HasSubstance(WATER_LAKE) || HasSubstance(WATER_SEA);
		}
		bool HasGrassMainSubstance() const {
			return GetMainSubstance() == GRASS;
		}
		bool HasDirtMainSubstance() const {
			return GetMainSubstance() == DIRT;
		}
		bool HasWaterMainSubstance() const {
			return GetMainSubstance() == WATER_LAKE || GetMainSubstance() == WATER_SEA;
		}

	public:
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
	static const size_t BIOM_CNT = 4;
	static const Biom Bioms[BIOM_CNT];
	static std::vector<Tile> BiomTiles[BIOM_CNT];
	static const size_t BIOM_BUILDING_MAP_CNT = 3;
	static const size_t BUILDING_MAP_TILES_CNT = 55;

public:
	static void AdjustTilesInfo();
	static int GetAppropriateTile(int leftTile, int upTile, int badTile = -1);
	static int GetSpriteIdByTile(const Tile& tile);
	static const Tile& GetTileBySpriteId(int spriteId);
	static ESubstance GetMainSubstanceBySpriteId(int spriteId);
private:
	static void AdjustNeighboursLeftRight(const Tile& l, const Tile& r);
	static void AdjustNeighboursUpDown(const Tile& u, const Tile& d);

private:
	enum {TILE_NEIGHBOUR_LEFT = 0, TILE_NEIGHBOUR_RIGHT, TILE_NEIGHBOUR_UP, TILE_NEIGHBOUR_DOWN, TILE_NEIGHBOUR_SIZE};
	static std::vector<int> TileNeighbours[TILE_NEIGHBOUR_SIZE][BUILDING_MAP_TILES_CNT];
	static std::vector<int> TempAppropriateTiles;
};