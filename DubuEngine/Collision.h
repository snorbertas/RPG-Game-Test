#pragma once
#include "Game.h"

/* ================================ collide ===============================
 *		Returns true if collision occurs between two objects or shapes.
 *
 *		Used to check collision.
 */
	bool PlayerMoveCollides(Player& pl,	// Player info
		Map& map,						// Map info
		int mov_x,						// Move x distance
		int mov_y);						// Move y distance


/* ================================ collide ===============================
 *		Returns true if collision occurs between two objects or shapes.
 *
 *		Used to check collision.
 */
	// Rectangle collision
	bool collide(int o1x, int o1y,	// Object 1 x, y coordinates
		int o1w, int o1h,			// Object 1 width and height
		int o2x, int o2y,			// Object 2 x, y coordinates
		int o2w, int o2h);			// Object 2 width and height

	// Circle collision
	bool collide(int o1x, int o1y,	// Object 1 x, y coordinates
		int o1r,					// Object 1 radius
		int o2x, int o2y,			// Object 2 x, y coordinates
		int o2r);					// Object 2 radius
	
	// CollisionBox collision
	bool collide(CollisionBox o1,	// Object 1 CollisionBox
		CollisionBox o2);			// Object 2 CollisionBox