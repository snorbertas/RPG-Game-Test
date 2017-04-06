#include "MiniMap.h"

#include "Game.h"
#include "Interfaces.h"
#include "ScaledDraw.h"

#include <allegro5/allegro_primitives.h>

const int TMiniMap::_BlockSize = 5;

void TMiniMap::Create(Game* g, SpriteStruct* sprites) {
	// Creating new minimap bitmap, freeing old one
	al_destroy_bitmap(_MiniMap);
	_MiniMap = al_create_bitmap(g->map.MAP_SIZE_X * _BlockSize, g->map.MAP_SIZE_Y * _BlockSize);

	// Saving display pointer and setting minimap to draw onto it
	auto display = al_get_current_display();
	al_set_target_bitmap(_MiniMap);

	// Draw Tiles
	for (int x = 0; x < g->map.MAP_SIZE_X; ++x) {
		for (int y = 0; y < g->map.MAP_SIZE_Y; ++y) {
			int drawX = x * _BlockSize;
			int drawY = y * _BlockSize;
			DrawTile(sprites, drawX, drawY, g->map.tile[x][y]);
		}
	}

	// Draw Trees
	for (const auto& obj : g->map.Objects) {
		if (obj.IsTree()) {
			int drawX = static_cast<int>(((double) obj.x / Map::TILE_SIZE) * _BlockSize);
			int drawY = static_cast<int>(((double) obj.y / Map::TILE_SIZE) * _BlockSize);
			DrawTree(sprites, drawX, drawY);
		}
	}

	// Returning to draw on display
	al_set_target_backbuffer(display);
}

void TMiniMap::DrawTile(SpriteStruct* sprites, int x, int y, int type) {
	auto substance = TilesInfo::GetMainSubstanceBySpriteId(type);
	int minimapSpriteId = -1;
	switch (substance) {
	case TilesInfo::GRASS:
		minimapSpriteId = 0;
		break;
	case TilesInfo::DIRT:
		minimapSpriteId = 1;
		break;
	case TilesInfo::WATER_LAKE:
		minimapSpriteId = 2;
		break;
	case TilesInfo::WATER_SEA:
		minimapSpriteId = 2;
		break;
	default:
		throw new exception("MiniMap: undefined tile type");
	}
	al_draw_bitmap(sprites->img_minimap[minimapSpriteId], x, y, 0);
}

void TMiniMap::DrawTree(SpriteStruct* sprites, int x, int y) {
	al_draw_bitmap(sprites->img_minimap[3], x, y, 0);
}

void TMiniMap::Draw(Game* g, SpriteStruct* sprites, int x, int y, int width, int height) {
	if (_MiniMap == nullptr) {
		Create(g, sprites);
	}

	// Variables
	int startX = -((g->camera.x + (g->BWIDTH / 2)) / Map::TILE_SIZE);
	int startY = -((g->camera.y + (g->BWIDTH / 2)) / Map::TILE_SIZE);

	/* Offset to center player
	  (this is due to camera being fitted for different resolution and visiblity */
	startX += 6;
	startY += 1;

	//int start_y = -g->camera.y / Map::TILE_SIZE;
	int hor_blocks = width / _BlockSize;
	int ver_blocks = height / _BlockSize;

	// Correct index
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	if (startX > (Map::MAP_SIZE_X - hor_blocks)) startX = Map::MAP_SIZE_X - hor_blocks;
	if (startY > (Map::MAP_SIZE_Y - ver_blocks)) startY = Map::MAP_SIZE_Y - ver_blocks;

	// Draw box
	DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, width + 16, height + 16);

	// Draw MiniMap
	x += 8;
	y += 8;
	int miniMapStartX = startX * _BlockSize;
	int miniMapStartY = startY * _BlockSize;
	auto subMiniMap = al_create_sub_bitmap(_MiniMap, miniMapStartX, miniMapStartY, width, height);
	DrawImage(g, subMiniMap, x, y, 0);

	// Draw player
	float player_x = (g->pl.x / Map::TILE_SIZE);
	float player_y = (g->pl.y / Map::TILE_SIZE);
	if (player_x >= startX && player_x <= startX + hor_blocks &&
		player_y >= startY && player_y <= startY + ver_blocks) {
		int draw_x = x + (player_x - startX) * _BlockSize;
		int draw_y = y + (player_y - startY) * _BlockSize;
		//DrawRectangle(g, draw_x, draw_y, block_size, block_size, 255, 255, 255);
		DrawImage(g, sprites->img_interface[SPRITE_INTERFACE_MAP_DOT], draw_x, draw_y, 0);
	}
}