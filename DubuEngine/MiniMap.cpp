#include "MiniMap.h"
#include "Interfaces.h"
#include "ScaledDraw.h"

void DrawMiniMapTile(Game* g, int type, int x, int y, int block_size) {
	if (TileIsWater(type)) {
		DrawRectangle(g, x, y, block_size, block_size, 99, 197, 207);
	} else
	if (TileIsGrass(type)) {
		DrawRectangle(g, x, y, block_size, block_size, 141, 196, 53);
	} else
	if (TileIsDirt(type)) {
		DrawRectangle(g, x, y, block_size, block_size, 194, 143, 92);
	}
}

void DrawMiniMapTree(Game* g, int x, int y) {
	DrawRectangle(g, x + 2, y + 2, 2, 1, 176, 128, 82);
	DrawRectangle(g, x + 1, y, 3, 2, 64, 156, 98);
}

void DrawMiniMap(Game* g, SpriteStruct* sprites, int x, int y, int w, int h) {
	// Variables
	static int block_size = 5;
	int start_x = -g->camera.x / Map::TILE_SIZE;
	int start_y = -g->camera.y / Map::TILE_SIZE;
	int width = w;
	int height = h;
	int hor_blocks = width / block_size;
	int ver_blocks = height / block_size;

	// Correct index
	if (start_x < 0) start_x = 0;
	if (start_y < 0) start_y = 0;
	if (start_x > (Map::MAP_SIZE_X - hor_blocks)) start_x = Map::MAP_SIZE_X - hor_blocks;
	if (start_y > (Map::MAP_SIZE_Y - ver_blocks)) start_y = Map::MAP_SIZE_Y - ver_blocks;

	// Draw box
	DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, width + 16, height + 16);

	// Draw tiles
	x += 8;
	y += 8;
	for (int xi = start_x; xi < start_x + hor_blocks; xi++) {
		for (int yi = start_y; yi < start_y + hor_blocks; yi++) {
			int draw_x = x + (xi - start_x) * block_size;
			int draw_y = y + (yi - start_y) * block_size;
			DrawMiniMapTile(g, g->map.tile[xi][yi], draw_x, draw_y, block_size);
		}
	}
	
	// Draw trees
	// Establish boundaries (using real values for faster comparison)
	int start_x_real = start_x * Map::TILE_SIZE;
	int start_y_real = start_y * Map::TILE_SIZE;
	int end_x_real = (start_x + hor_blocks) * Map::TILE_SIZE;
	int end_y_real = (start_y + ver_blocks) * Map::TILE_SIZE;

	// Loop trough all map objects (is there a faster way?)
	for (int i = 0; i < g->map.Objects.size(); i++) {
		MapObjectInfo::MapObject* mapobj = &g->map.Objects[i];
		if (mapobj->x >= start_x_real && mapobj->x <= end_x_real &&
			mapobj->y >= start_y_real && mapobj->y <= end_y_real) {

			// If it's a tree, draw it
			if (mapobj->IsTree() || mapobj->IsBigTree()) {
				int draw_x = x + (((float)mapobj->x / Map::TILE_SIZE) - start_x) * block_size;
				int draw_y = y + (((float)mapobj->y / Map::TILE_SIZE) - start_y) * block_size;
				DrawMiniMapTree(g, draw_x, draw_y);
			}
		}
	}

	// Draw player
	float player_x = (g->pl.x / Map::TILE_SIZE);
	float player_y = (g->pl.y / Map::TILE_SIZE);
	if (player_x >= start_x && player_x <= start_x + hor_blocks &&
		player_y >= start_y && player_y <= start_y + ver_blocks) {
		int draw_x = x + (player_x - start_x) * block_size;
		int draw_y = y + (player_y - start_y) * block_size;
		//DrawRectangle(g, draw_x, draw_y, block_size, block_size, 255, 255, 255);
		DrawImage(g, sprites->img_interface[SPRITE_INTERFACE_MAP_DOT], draw_x, draw_y, 0);
	}
}