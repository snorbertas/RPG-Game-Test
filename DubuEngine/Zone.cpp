#include "Game.h"
#include "ScaledDraw.h"

void Territory::Recalculate(Map* map, int new_owner) {
	// Update target owner if required
	if (new_owner > -2) owner = new_owner;

	// Simple recalculation
	zone.clear();
	for (int x = 0; x < Map::MAP_SIZE_X; x++) {
		for (int y = 0; y < Map::MAP_SIZE_Y; y++) {
			if (map->zone[x][y].Owner == new_owner) {
				zone.push_back(std::make_pair(x, y));
			}
		}
	}
}

void RenderTerritoryInfo(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font) {
	// Ratios for font
	float r_x = (float)g->s_x / (float)g->BWIDTH;
	float r_y = (float)g->s_y / (float)g->BHEIGHT;

	// We could do it according to player's coordinates or mouse position
	// MOUSE POSITION
	int x = round((g->pos_x - g->camera.x) / Map::TILE_SIZE);
	int y = round((g->pos_y - g->camera.y) / Map::TILE_SIZE);
	if (x >= 0 && x < Map::MAP_SIZE_X && y >= 0 && y < Map::MAP_SIZE_Y) {
		// Draw
		int target_owner = g->map.zone[x][y].Owner;
		if (g->map.zone[x][y].Owner > -2) {
			// Find the correct territory
			for (size_t i = 0; i < g->map.territory.size(); i++) {
				if (g->map.territory[i].owner == target_owner) {
					// Found correct territory
					for (size_t z = 0; z < g->map.territory[i].zone.size(); z++) {
						// Draw
						int sprite_id = SPRITE_ZONE_MINE;
						if (target_owner >= 0) sprite_id = SPRITE_ZONE_FOREIGN;
						DrawImage(g, sprites->img_interface[sprite_id],
							(g->map.territory[i].zone[z].first * Map::TILE_SIZE) + g->camera.x,
							(g->map.territory[i].zone[z].second * Map::TILE_SIZE) + g->camera.y,
							0);
					}
				}
			}

			// Text
			std::string owner_name = g->pl.name;
			if (target_owner >= 0) owner_name = g->Players[target_owner].name;
			DrawRectangle(g, g->pos_x + 2, g->pos_y, 200, 18, 0, 0, 0, 0.5);
			DrawRectangle(g, g->pos_x + 3, g->pos_y + 1, 198, 16, 0, 0, 0, 0.3);
			DrawText(font[2], 255, 255, 255, g->pos_x + 24, g->pos_y - 2, ALLEGRO_ALIGN_LEFT,
				"Owner: %s", owner_name.c_str());
		}
	}
}