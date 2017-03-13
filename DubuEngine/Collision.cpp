#include "Collision.h"

#include <algorithm>

bool PlayerMoveCollides(Player& pl, Map& map, double mov_x, double mov_y) {
	// Player box
	CollisionBox o1(pl.x + mov_x, pl.y + mov_y, pl.w, pl.h);

	// Adjust the box
	o1.x += 28;
	o1.y += 56;
	o1.w = 8;
	o1.h = 8;

	// Check collision with map boundary
	if (o1.x < 0 || o1.x + o1.w > (Map::MAP_SIZE_X * Map::TILE_SIZE) ||
		o1.y < 0 || o1.y + o1.h > (Map::MAP_SIZE_Y * Map::TILE_SIZE)) {
		return true;
	}

	int xMin = std::max(static_cast<int>(o1.x) - 2 * Map::TILE_SIZE, 0);
	int xMax = static_cast<int>(o1.x) + o1.w;
	CollisionBox minBox(xMin, 0, 0, 0);
	CollisionBox maxBox(xMax, Map::MAP_SIZE_Y * Map::TILE_SIZE, 0, 0);
	
	// Loop through all except collision objects
	auto lft = lower_bound(map.except_solid.begin(), map.except_solid.end(), minBox);
	auto rht = upper_bound(map.except_solid.begin(), map.except_solid.end(), maxBox);
	for (auto it = lft; it != rht; ++it) {
		auto& exceptSolid = *it;
		if (collide(o1, exceptSolid)) {
			return false;
		}
	}

	// Loop trough all collision objects
	lft = lower_bound(map.solid.begin(), map.solid.end(), minBox);
	rht = upper_bound(map.solid.begin(), map.solid.end(), maxBox);
	for (auto it = lft; it != rht; ++it) {
		auto& solid = *it;
		if (collide(o1, solid)) {
			return true;
		}
	}

	return false;
}

bool collide(CollisionBox o1, CollisionBox o2) {
	if (o1.x <= o2.x + o2.w &&
		o1.x + o1.w >= o2.x &&
		o1.y <= o2.y + o2.h &&
		o1.h + o1.y >= o2.y) {
		return true;
	}
	return false;
}

bool collide(int o1x, int o1y, int o1w, int o1h, int o2x, int o2y, int o2w, int o2h) {
	if (o1x <= o2x + o2w &&
		o1x + o1w >= o2x &&
		o1y <= o2y + o2h &&
		o1h + o1y >= o2y) {
		return true;
	}
	return false;
}

bool collide(int o1x, int o1y, int o1r, int o2x, int o2y, int o2r) {
	int dx = o1x - o2x;
	int dy = o1y - o2y;
	int distance = sqrt(dx * dx + dy * dy);

	if (distance < o1r + o2r) {
		return true;
	}
	return false;
}