#include "Collision.h"

bool PlayerMoveCollides(Player& pl, Map& map, double mov_x, double mov_y) {
	// Player box
	CollisionBox o1(pl.x + mov_x, pl.y + mov_y, pl.w, pl.h);

	// Adjust the box
	o1.x += 28;
	o1.y += 56;
	o1.w = 8;
	o1.h = 8;

	// Loop trough all collision objects
	for (int i = 0; i < map.solid.size(); i++) {
		if (collide(o1, map.solid[i])) {
			// Let's make sure it's not excepted
			bool collision_found = true;
			for (int e = 0; e < map.except_solid.size(); e++) {
				if (collide(o1, map.except_solid[e])) {
					collision_found = false;
				}
			}
			if(collision_found) return true;
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