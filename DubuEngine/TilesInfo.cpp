#include "TilesInfo.h"
#include <iostream>

std::vector<int> TilesInfo::TileNeighbours[TILE_NEIGHBOUR_SIZE][MAX_TILE_SPRITES]{};

std::vector<int> TilesInfo::TempAppropriateTiles{};

int TilesInfo::GetBiomStart(int biom) {
	return biom * BiomSize;
}

bool TilesInfo::InBiom(int biom, int tileIndex) {
	int biomStart = GetBiomStart(biom);
	int biomEnd = GetBiomStart(biom + 1) - 1;
	return tileIndex >= biomStart && tileIndex <= biomEnd;
}

int TilesInfo::MoveTileIndexToBiom(int biom, int tileIndex) {
	return tileIndex + biom * BiomSize;
}

void TilesInfo::AdjustTilesInfo(int biom, int outerBiom) {
}

void TilesInfo::AdjustTilesInfo(int biom, int outerBiom, int forwardDir, int backwardDir, const std::vector<std::pair<int, int>>& neighbourPairs) {
	for (auto p : neighbourPairs) {
		int a = p.first;
		int b = p.second;

		if (InBiom(0, b)) {
			a = MoveTileIndexToBiom(biom, a);
			b = MoveTileIndexToBiom(biom, b);
		} else {
			a = MoveTileIndexToBiom(biom, a);
			b = outerBiom * BiomSize + (b - BiomSize);
		}

		TileNeighbours[forwardDir][a].push_back(b);
		TileNeighbours[backwardDir][b].push_back(a);
	}
}

void TilesInfo::AdjustTilesInfo() {
	AdjustTilesInfo(0, 1);
	AdjustTilesInfo(1, 0);
	AdjustTilesInfo(2, 0);
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
		if (tile != badTile && AppropriateNeighbours(tile, leftTile, TILE_NEIGHBOUR_LEFT) && AppropriateNeighbours(tile, upTile, TILE_NEIGHBOUR_UP)) {
			TempAppropriateTiles.push_back(tile);
		}
	}
	if (TempAppropriateTiles.size() == 0) {
		std::cout << "No Appropriate Tiles" << std::endl;
		std::cout << "Left Tile: " << leftTile << std::endl;
		std::cout << "Up Tile: " << upTile << std::endl;
		return -1;
	}
	int resultIndex = rand() % ((int) TempAppropriateTiles.size());
	return TempAppropriateTiles[resultIndex];
}