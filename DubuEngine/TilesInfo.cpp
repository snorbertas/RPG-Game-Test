#include "TilesInfo.h"

const TilesInfo::Biom TilesInfo::Bioms[TilesInfo::BiomCnt] = {Biom(TilesInfo::GRASS, TilesInfo::DIRT),
															  Biom(TilesInfo::DIRT,  TilesInfo::GRASS),
															  Biom(TilesInfo::WATER, TilesInfo::GRASS)};

TilesInfo::Tile TilesInfo::BiomTiles[BiomCnt][BiomTileCnt]{};

std::vector<int> TilesInfo::TileNeighbours[TILE_NEIGHBOUR_SIZE][MAX_TILE_SPRITES]{};

std::vector<int> TilesInfo::TempAppropriateTiles{};

void TilesInfo::Biom::CreateTiles(Tile* tiles) const {
	Tile midTile;
	for (int i = 0; i < 9; ++i)
		midTile.Side[i] = InnerSubstance;

	Tile tile = midTile;
	tile.Side[0] = tile.Side[1] = tile.Side[2] = tile.Side[3] = tile.Side[6] = OuterSubstance;
	tiles[0] = tile;
	tile.Side[3] = tile.Side[6] = InnerSubstance;
	tiles[1] = tile;
	tile.Side[5] = tile.Side[8] = OuterSubstance;
	tiles[2] = tile;

	tile = midTile;
	tile.Side[0] = tile.Side[3] = tile.Side[6] = OuterSubstance;
	tiles[3] = tile;
	tiles[4] = midTile;
	tile = midTile;
	tile.Side[2] = tile.Side[5] = tile.Side[8] = OuterSubstance;
	tiles[5] = tile;

	tile = midTile;
	tile.Side[0] = tile.Side[3] = tile.Side[6] = tile.Side[7] = tile.Side[8] = OuterSubstance;
	tiles[6] = tile;
	tile.Side[0] = tile.Side[3] = InnerSubstance;
	tiles[7] = tile;
	tile.Side[2] = tile.Side[5] = OuterSubstance;
	tiles[8] = tile;

	tile = midTile;
	tile.Side[8] = OuterSubstance;
	tiles[9] = tile;
	tile = midTile;
	tile.Side[6] = OuterSubstance;
	tiles[10] = tile;
	tile = midTile;
	tile.Side[2] = OuterSubstance;
	tiles[11] = tile;
	tile = midTile;
	tile.Side[0] = OuterSubstance;
	tiles[12] = tile;

	int spriteStart = 0;
	if (InnerSubstance == DIRT)
		spriteStart = 15;
	if (InnerSubstance == WATER)
		spriteStart = 28;
	for (int i = 0; i < BiomTileCnt; ++i)
		tiles[i].SpriteId = spriteStart + i;
	if (InnerSubstance == GRASS)
		for (int i = 5; i < BiomTileCnt; ++i)
			tiles[i].SpriteId = i + 2;
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
	for (int i = 0; i < BiomCnt; ++i) {
		Bioms[i].CreateTiles(BiomTiles[i]);
	}

	for (int i = 0; i < BiomCnt; ++i) {
		for (int ti = 0; ti < BiomTileCnt; ++ti) {
			for (int tj = ti + 1; tj < BiomTileCnt; ++tj) {
				AdjustNeighboursLeftRight(BiomTiles[i][ti], BiomTiles[i][tj]);
				AdjustNeighboursLeftRight(BiomTiles[i][tj], BiomTiles[i][ti]);
				AdjustNeighboursUpDown(BiomTiles[i][ti], BiomTiles[i][tj]);
				AdjustNeighboursUpDown(BiomTiles[i][tj], BiomTiles[i][ti]);
			}
		}
		for (int ti = 0; ti < BiomTileCnt; ++ti) {
			AdjustNeighboursLeftRight(BiomTiles[i][ti], BiomTiles[i][ti]);
			AdjustNeighboursUpDown(BiomTiles[i][ti], BiomTiles[i][ti]);
		}
	}

	for (int i = 0; i < BiomCnt; ++i) {
		for (int j = i + 1; j < BiomCnt; ++j) {
			for (int ti = 0; ti < BiomTileCnt; ++ti) {
				for (int tj = 0; tj < BiomTileCnt; ++tj) {
					AdjustNeighboursLeftRight(BiomTiles[i][ti], BiomTiles[j][tj]);
					AdjustNeighboursLeftRight(BiomTiles[j][tj], BiomTiles[i][ti]);
					AdjustNeighboursUpDown(BiomTiles[i][ti], BiomTiles[j][tj]);
					AdjustNeighboursUpDown(BiomTiles[j][tj], BiomTiles[i][ti]);
				}
			}
		}
	}
}

bool TilesInfo::AppropriateNeighbours(int tile, int neighbourTile, int direction) {
	if (neighbourTile == -1)
		return true;
	for (auto curNeighbour : TileNeighbours[direction][tile])
		if (curNeighbour == neighbourTile)
			return true;
	return false;
}

int TilesInfo::GetAppropriateTile(int leftTile, int upTile, int badTile) {
	TempAppropriateTiles.clear();
	for (int tile = 0; tile < MAX_TILE_SPRITES; ++tile) {
		if (tile != badTile && 
			AppropriateNeighbours(tile, leftTile, TILE_NEIGHBOUR_LEFT) && 
			AppropriateNeighbours(tile, upTile, TILE_NEIGHBOUR_UP)) {
			TempAppropriateTiles.push_back(tile);
		}
	}
	if (TempAppropriateTiles.size() == 0) {
		return -1;
	}
	int resultIndex = abs(rand()) % ((int) TempAppropriateTiles.size());
	return TempAppropriateTiles[resultIndex];
}