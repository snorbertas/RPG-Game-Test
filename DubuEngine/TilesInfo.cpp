#include "TilesInfo.h"

#include <algorithm>

const TilesInfo::Biom TilesInfo::Bioms[TilesInfo::BIOM_CNT] = {	Biom(TilesInfo::GRASS, TilesInfo::DIRT),
																Biom(TilesInfo::DIRT,  TilesInfo::GRASS),
																Biom(TilesInfo::WATER_LAKE, TilesInfo::GRASS),
																Biom(TilesInfo::WATER_SEA, TilesInfo::GRASS)	};

std::vector<TilesInfo::Tile> TilesInfo::BiomTiles[BIOM_CNT]{};

std::vector<int> TilesInfo::TileNeighbours[TILE_NEIGHBOUR_SIZE][BUILDING_MAP_TILES_CNT]{};

std::vector<int> TilesInfo::TempAppropriateTiles{};

void TilesInfo::Biom::CreateTiles(std::vector<Tile>& tiles) const {
	Tile midTile;
	for (size_t i = 0; i < 9; ++i)
		midTile.Side[i] = InnerSubstance;

	Tile tile = midTile;
	tile.Side[0] = tile.Side[1] = tile.Side[2] = tile.Side[3] = tile.Side[6] = OuterSubstance;
	tiles.push_back(tile);
	tile.Side[3] = tile.Side[6] = InnerSubstance;
	tiles.push_back(tile);
	tile.Side[5] = tile.Side[8] = OuterSubstance;
	tiles.push_back(tile);

	tile = midTile;
	tile.Side[0] = tile.Side[3] = tile.Side[6] = OuterSubstance;
	tiles.push_back(tile);
	tiles.push_back(midTile);
	tile = midTile;
	tile.Side[2] = tile.Side[5] = tile.Side[8] = OuterSubstance;
	tiles.push_back(tile);

	tile = midTile;
	tile.Side[0] = tile.Side[3] = tile.Side[6] = tile.Side[7] = tile.Side[8] = OuterSubstance;
	tiles.push_back(tile);
	tile.Side[0] = tile.Side[3] = InnerSubstance;
	tiles.push_back(tile);
	tile.Side[2] = tile.Side[5] = OuterSubstance;
	tiles.push_back(tile);

	tile = midTile;
	tile.Side[8] = OuterSubstance;
	tiles.push_back(tile);
	tile = midTile;
	tile.Side[6] = OuterSubstance;
	tiles.push_back(tile);
	tile = midTile;
	tile.Side[2] = OuterSubstance;
	tiles.push_back(tile);
	tile = midTile;
	tile.Side[0] = OuterSubstance;
	tiles.push_back(tile);

	int spriteStart;
	if (InnerSubstance == GRASS)
		spriteStart = 0;
	else if (InnerSubstance == DIRT)
		spriteStart = 15;
	else if (InnerSubstance == WATER_LAKE)
		spriteStart = 28;
	else if (InnerSubstance == WATER_SEA)
		spriteStart = 55;
	else
		throw std::exception("TilesInfo: Unknown inner substance");
	for (size_t i = 0; i < 13; ++i)
		tiles[i].SpriteId = spriteStart + i;
	if (InnerSubstance == GRASS)
		for (size_t i = 5; i < 13; ++i)
			tiles[i].SpriteId = i + 2;

	if (InnerSubstance == DIRT) {
		tile = tiles[0];
		tile.Side[8] = GRASS;
		tiles.push_back(tile);
		tile = tiles[2];
		tile.Side[6] = GRASS;
		tiles.push_back(tile);
		tile = tiles[6];
		tile.Side[2] = GRASS;
		tiles.push_back(tile);
		tile = tiles[8];
		tile.Side[0] = GRASS;
		tiles.push_back(tile);
		tile = midTile;
		tile.Side[0] = tile.Side[8] = GRASS;
		tiles.push_back(tile);
		tile = midTile;
		tile.Side[2] = tile.Side[6] = GRASS;
		tiles.push_back(tile);
		tile = tiles[1];
		tile.Side[6] = GRASS;
		tiles.push_back(tile);
		tile = tiles[1];
		tile.Side[8] = GRASS;
		tiles.push_back(tile);
		tile = tiles[3];
		tile.Side[2] = GRASS;
		tiles.push_back(tile);
		tile = tiles[3];
		tile.Side[8] = GRASS;
		tiles.push_back(tile);
		tile = tiles[5];
		tile.Side[0] = GRASS;
		tiles.push_back(tile);
		tile = tiles[5];
		tile.Side[6] = GRASS;
		tiles.push_back(tile);
		tile = tiles[7];
		tile.Side[0] = GRASS;
		tiles.push_back(tile);
		tile = tiles[7];
		tile.Side[2] = GRASS;
		tiles.push_back(tile);

		for (size_t i = 13; i < tiles.size(); ++i)
			tiles[i].SpriteId = 41 + (i - 13);
	}
}

void TilesInfo::AdjustNeighboursLeftRight(const Tile& l, const Tile& r) {
	if (l.Side[2] == r.Side[0] && l.Side[5] == r.Side[3] && l.Side[8] == r.Side[6]) {
		TileNeighbours[TILE_NEIGHBOUR_RIGHT][l.SpriteId].push_back(r.SpriteId);
		TileNeighbours[TILE_NEIGHBOUR_LEFT][r.SpriteId].push_back(l.SpriteId);
	}
}

void TilesInfo::AdjustNeighboursUpDown(const Tile& u, const Tile& d) {
	if (u.Side[6] == d.Side[0] && u.Side[7] == d.Side[1] && u.Side[8] == d.Side[2]) {
		TileNeighbours[TILE_NEIGHBOUR_DOWN][u.SpriteId].push_back(d.SpriteId);
		TileNeighbours[TILE_NEIGHBOUR_UP][d.SpriteId].push_back(u.SpriteId);
	}
}

void TilesInfo::AdjustTilesInfo() {
	for (int i = 0; i < BIOM_CNT; ++i) {
		Bioms[i].CreateTiles(BiomTiles[i]);
	}

	for (int i = 0; i < BIOM_BUILDING_MAP_CNT; ++i) {
		int size = (int) BiomTiles[i].size();
		for (int ti = 0; ti < size; ++ti) {
			for (int tj = ti + 1; tj < size; ++tj) {
				AdjustNeighboursLeftRight(BiomTiles[i][ti], BiomTiles[i][tj]);
				AdjustNeighboursLeftRight(BiomTiles[i][tj], BiomTiles[i][ti]);
				AdjustNeighboursUpDown(BiomTiles[i][ti], BiomTiles[i][tj]);
				AdjustNeighboursUpDown(BiomTiles[i][tj], BiomTiles[i][ti]);
			}
		}
		for (int ti = 0; ti < size; ++ti) {
			AdjustNeighboursLeftRight(BiomTiles[i][ti], BiomTiles[i][ti]);
			AdjustNeighboursUpDown(BiomTiles[i][ti], BiomTiles[i][ti]);
		}
	}

	for (int i = 0; i < BIOM_BUILDING_MAP_CNT; ++i) {
		int sizeI = (int) BiomTiles[i].size();
		for (int j = i + 1; j < BIOM_BUILDING_MAP_CNT; ++j) {
			int sizeJ = (int) BiomTiles[j].size();
			for (int ti = 0; ti < sizeI; ++ti) {
				for (int tj = 0; tj < sizeJ; ++tj) {
					AdjustNeighboursLeftRight(BiomTiles[i][ti], BiomTiles[j][tj]);
					AdjustNeighboursLeftRight(BiomTiles[j][tj], BiomTiles[i][ti]);
					AdjustNeighboursUpDown(BiomTiles[i][ti], BiomTiles[j][tj]);
					AdjustNeighboursUpDown(BiomTiles[j][tj], BiomTiles[i][ti]);
				}
			}
		}
	}

	for (int i = 0; i != TILE_NEIGHBOUR_SIZE; ++i)
		for (int j = 0; j < BUILDING_MAP_TILES_CNT; ++j)
			std::sort(TileNeighbours[i][j].begin(), TileNeighbours[i][j].end());
}

int TilesInfo::GetSpriteIdByTile(const Tile& tile) {
	int biomIndex = 0;
	switch (tile.GetMainSubstance()) {
	case GRASS:
		biomIndex = 0;
		break;
	case DIRT:
		biomIndex = 1;
		break;
	case WATER_LAKE:
		biomIndex = 2;
		break;
	case WATER_SEA:
		biomIndex = 3;
		break;
	default:
		throw std::exception("GetSpriteByTile: unknown substance");
	}
	int size = (int) BiomTiles[biomIndex].size();
	for (int i = 0; i < size; ++i) {
		if (BiomTiles[biomIndex][i] == tile) {
			return BiomTiles[biomIndex][i].SpriteId;
		}
	}
	throw std::exception("GetSpriteByTile: no suitable tile found");
}

const TilesInfo::Tile& TilesInfo::GetTileBySpriteId(int spriteId) {
	int biomIndex;
	int tileIndex = spriteId;
	if (spriteId >= 55) {
		biomIndex = 3;
		tileIndex -= 55;
	} else if (spriteId >= 41) {
		biomIndex = 1;
		tileIndex = 13 + (spriteId - 41);
	} else if (spriteId >= 28) {
		biomIndex = 2;
		tileIndex -= 28;
	} else if (spriteId >= 15) {
		biomIndex = 1;
		tileIndex -= 15;
	} else {
		biomIndex = 0;
		if (spriteId == 5 || spriteId == 6)
			tileIndex = 4;
		else if (spriteId >= 7)
			tileIndex -= 2;
	}
	return BiomTiles[biomIndex][tileIndex];
}

TilesInfo::ESubstance TilesInfo::GetMainSubstanceBySpriteId(int spriteId) {
	return GetTileBySpriteId(spriteId).GetMainSubstance();
}

static int TryAppropriateTile(std::vector<int>& TempAppropriateTiles, int type) {
	if (type == 0) {
		bool contains4 = false;
		for (const auto& tile : TempAppropriateTiles) {
			if (tile == 4) {
				contains4 = true;
				break;
			}
		}
		if (contains4)
			return 4;
		else
			return -1;
	} else {
		bool have32 = false;
		bool haveProper = false;
		for (int i = 0; i < (int) TempAppropriateTiles.size(); ++i) {
			if (!(TempAppropriateTiles[i] >= 28 && TempAppropriateTiles[i] <= 40)) {
				TempAppropriateTiles[i] = TempAppropriateTiles.back();
				TempAppropriateTiles.pop_back();
				--i;
			} else {
				have32 |= (TempAppropriateTiles[i] == 32);
				haveProper |= (TempAppropriateTiles[i] < 37);
			}
		}
		if (TempAppropriateTiles.size() == 0)
			return -1;
		if (have32 && rand() % 10 < 8) {
			return 32;
		}
		if (haveProper && rand() % 10 < 5) {
			for (int i = 0; i < (int) TempAppropriateTiles.size(); ++i) {
				if (TempAppropriateTiles[i] >= 37) {
					TempAppropriateTiles[i] = TempAppropriateTiles.back();
					TempAppropriateTiles.pop_back();
					--i;
				}
			}
		}
		return TempAppropriateTiles[static_cast<size_t>(rand()) % TempAppropriateTiles.size()];
	}
}

int TilesInfo::GetAppropriateTile(int leftTile, int upTile, int badTile) {
	TempAppropriateTiles.clear();

	if (leftTile == -1 && upTile == -1) {
		for (int i = 0; i < BUILDING_MAP_TILES_CNT; ++i)
			if (i != badTile)
				TempAppropriateTiles.push_back(i);
	} else if (leftTile == -1) {
		TempAppropriateTiles = TileNeighbours[TILE_NEIGHBOUR_DOWN][upTile];
	} else if (upTile == -1) {
		TempAppropriateTiles = TileNeighbours[TILE_NEIGHBOUR_RIGHT][leftTile];
	} else {
		auto& leftNeighbours = TileNeighbours[TILE_NEIGHBOUR_RIGHT][leftTile];
		auto& upNeighbours = TileNeighbours[TILE_NEIGHBOUR_DOWN][upTile];
		size_t leftNeighboursSize = leftNeighbours.size();
		size_t upNeighboursSize = upNeighbours.size();
		for (size_t i = 0, j = 0; i < leftNeighboursSize && j < upNeighboursSize; ) {
			if (leftNeighbours[i] < upNeighbours[j])
				++i;
			else if (leftNeighbours[i] > upNeighbours[j])
				++j;
			else {
				if (leftNeighbours[i] != badTile)
					TempAppropriateTiles.push_back(leftNeighbours[i]);
				++i;
				++j;
			}
		}
	}

	if (TempAppropriateTiles.size() == 0) {
		return -1;
	}
	
	int type = rand() % 100;
	int chance = 1;
	bool waterConnection = false;
	if (leftTile != -1 && GetMainSubstanceBySpriteId(leftTile) == WATER_LAKE && (leftTile >= 37 || (leftTile <= 32 && leftTile != 30)))
		waterConnection = true;
	if (upTile != -1 && GetMainSubstanceBySpriteId(upTile) == WATER_LAKE && (upTile >= 37 || (upTile <= 32 && upTile != 30)))
		waterConnection = true;
	if (waterConnection)
		chance = 99;
	if (type < chance)
		type = 2;
	else
		type = 0;
	int result = TryAppropriateTile(TempAppropriateTiles, type);
	if (result == -1) {
		type = (type + 2) % 4;
		result = TryAppropriateTile(TempAppropriateTiles, type);
	}
	return result;
}