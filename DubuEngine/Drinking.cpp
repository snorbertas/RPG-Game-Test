#include "Drinking.h"
#include "Game.h"
#include "Collision.h"

bool PlayerNearWater(Game* g, Player* p) {
	// I will take a small 3 x 3 grid around the player
	int current_x = g->pl.x / Map::TILE_SIZE;
	int current_y = g->pl.y / Map::TILE_SIZE;

	// Error check
	if (current_x < 0 || current_x >= Map::MAP_SIZE_X ||
		current_y < 0 || current_y >= Map::MAP_SIZE_Y) {

		// Return false if player isnt within map boundaries
		return false;
	}

	// Start of grid
	int start_x = current_x - 1;
	int start_y = current_y - 1;
	if (start_x < 0) start_x = 0;
	if (start_y < 0) start_y = 0;

	// End of grid
	int end_x = start_x + 2;
	int end_y = start_y + 2;
	if (end_x > Map::MAP_SIZE_X - 1) end_x = Map::MAP_SIZE_X - 1;
	if (end_y > Map::MAP_SIZE_Y - 1) end_y = Map::MAP_SIZE_Y - 1;

	/*  This calculation assumes that the player is within the map grid
		If the player glitched outside the map then the closest 3x3 grid will be used. */

	for (int x = start_x; x <= end_x; x++) {
		for (int y = start_y; y <= end_y; y++) {
			int tile_id = g->map.tile[x][y];
			if (TileIsWater(tile_id)) {
				// Collision boxes (solid water and 1x1 center of player)
				CollisionBox water = GetCollisionFromTile(tile_id, x, y);
				CollisionBox target = CollisionBox(p->x + (p->w / 2), p->y + (p->h / 2), 1, 1);
				
				// Move target according to direction player faces
				if (p->facing == Player::FacingRight) {
					target.x += Map::TILE_SIZE / 4;
				}
				if (p->facing == Player::FacingLeft) {
					target.x -= Map::TILE_SIZE / 4;
				}
				if (p->facing == Player::FacingUp) {
					// Facing up is fine by default
				}
				if (p->facing == Player::FacingDown) {
					target.y += Map::TILE_SIZE / 1.5;
				}

				// Check collision
				if (collide(target, water)) {
					// Player is close enough water to drink
					return true;
				}
			}
		}
	}
	return false;
}

void StartDrinking(Game* g, Player* p) {
	// Local player
	if (p->pID == -1) {
		// Make sure we're facing water
		if (PlayerNearWater(g, p)) {
			p->drinking = true;
			p->drink_timer = p->drink_duration;
		}
	}
}

void HandleDrinking(Game* g, Player* p) {
	if (!p->drinking)
		return;

	--p->drink_timer;
	if (p->drink_timer <= 0) {
		p->drinking = false;
		if (p->pID == -1) {
			p->pee_ammo = 10;
			AddChatMessage(g->chat, "__SYSTEM__", 139, 49, 121, "Refilled Bladder");
		}
	}
}

void RenderDrinking(Game * g, SpriteStruct* sprites) {
}