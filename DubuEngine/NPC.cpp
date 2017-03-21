#include "Game.h"
#include "Collision.h"
#include "GameHandler.h"

bool NPC::PlayerInRange(Player& p) {
	CollisionBox col_player(p.x, p.y, p.w, p.h);
	CollisionBox col_aggression_zone(
		x - aggression_radius,
		y - aggression_radius,
		aggression_radius * 2,
		aggression_radius * 2);
	if (collide(col_player, col_aggression_zone)) {
		return true;
	} else {
		return false;
	}
}

Node NPC::PickRandomDestination(Map& map) {
	// Set starting x and y coordinates on map
	Node start(MapX() - roam_radius,
		MapY() - roam_radius);

	// Correction to prevent index errors
	if (start.x < 0) start.x = 0;
	if (start.y < 0) start.y = 0;
	if (start.x > Map::MAP_SIZE_X - roam_radius)
		start.x = Map::MAP_SIZE_X - roam_radius;
	if (start.y > Map::MAP_SIZE_Y - roam_radius)
		start.y = Map::MAP_SIZE_Y - roam_radius;

	// Choose random destination
	Node dest(start.x + rand() % roam_radius,
		start.y + rand() % roam_radius);

	// Check if pathable
	if (TileIsPathable(map.tile[dest.x][dest.y])) {
		return Node(dest.x, dest.y);
	} else {
		// Invalid tile, NPC will idle for one more tick and its chance again
		return Node(MapX(), MapY());
	}
}

void NPC::HandleAI(Game* g) {
	/* The AI has to be smart enough to develop agression
	and to think of a path towards to find player without
	collision */

	// Reset movement
	left = false;
	right = false;
	up = false;
	down = false;

	// Idle
	if (idle) {
		idle_timer--;
		if (idle_timer <= 0) {
			// Stop idling
			idle = false;

			// Find new path to roam to
			path = FindPath(g->map, GetPositionNode(), PickRandomDestination(g->map));
			if (!path.successful) {
				// Continue idling
				idle = true;
				idle_timer = 1;
			}
		}
	}

	// Aggresion
	if ((path.current_node > 1 || idle) && PlayerInRange(g->pl)) {
		// Findpath towards player
		path = FindPath(g->map, GetPositionNode(), g->pl.GetPositionNode(), 10);
		idle = false;
	}

	// Check path
	if (path.successful) {
		// Decide path node
		Node goal = path.node[path.current_node];

		// Decide movement
		if ((x + (w / 2)) - goal.x < -4) {
			right = true;
			facing = Player::Facing::FacingRight;
		}
		if ((x + (w / 2)) - goal.x > 4) {
			left = true;
			facing = Player::Facing::FacingLeft;
		}
		if ((y + (h / 2)) - goal.y < -4) {
			down = true;
			facing = Player::Facing::FacingDown;
		}
		if ((y + (h / 2)) - goal.y > 4) {
			up = true;
			facing = Player::Facing::FacingUp;
		}

		// Check if we met our current node goal yet (might need to tweak these boxes)
		CollisionBox col_npc(x + (w / 2) - 8, y + (h / 2) - 8, 16, 16);
		CollisionBox col_goal(goal.x - 8, goal.y - 8, 16, 16);
		if (collide(col_npc, col_goal)) {
			// Reached our current node goal, what's next?
			path.current_node++;

			if ((size_t)path.current_node >= path.node.size()) {
				// Completed whole path
				path.Reset();

				// Chill for few seconds
				idle = true;
				idle_timer = SecondsToTicks(idle_min_duration + rand() % idle_extra_rand_duration);
			}
		}

	} else {
		idle = true;
	}
}

void NPC::UpdateMovementSprite() {
	sprite_frame++;

	// Correct frames
	switch (facing) {
	case Player::FacingUp:
		if (sprite_frame < Player::FrameUp_0 ||
			sprite_frame > Player::FrameUp_2) {
			sprite_frame = 6;
		}
		break;
	case Player::FacingDown:
		if (sprite_frame < Player::FrameDown_0 ||
			sprite_frame > Player::FrameDown_2) {
			sprite_frame = 0;
		}
		break;
	case Player::FacingLeft:
		if (sprite_frame < Player::FrameHor_0 ||
			sprite_frame > Player::FrameHor_2) {
			sprite_frame = 3;
		}
		break;
	case Player::FacingRight:
		if (sprite_frame < Player::FrameHor_0 ||
			sprite_frame > Player::FrameHor_2) {
			sprite_frame = 3;
		}
		break;
	}
}


void NPC::HandleIdle() {
	// Update anim counter
	ticks_left_anim--;		
		
	// Animation
	if (ticks_left_anim <= 0) {
		// Update the animation
		if (facing == Player::FacingDown) {
			if (sprite_frame != Player::FrameIdleDown_0) {
				sprite_frame = Player::FrameIdleDown_0;
			} else {
				sprite_frame = Player::FrameIdleDown_1;
			}
		} else if (facing == Player::FacingLeft ||
					facing == Player::FacingRight) {
			if (sprite_frame != Player::FrameIdleHor_0) {
				sprite_frame = Player::FrameIdleHor_0;
			} else {
				sprite_frame = Player::FrameIdleHor_1;
			}
		} else if (facing == Player::FacingUp) {
			if (sprite_frame != Player::FrameIdleUp_0) {
				sprite_frame = Player::FrameIdleUp_0;
			} else {
				sprite_frame = Player::FrameIdleUp_1;
			}
		}

		// Reset anim ticks
		ticks_left_anim = ticks_to_anim * 2;
	}
}

void NPC::HandleMovementLogic() {
	// Update tick counters
	ticks_left_move--;
	ticks_left_anim--;

	// Time to mobilize
	if (ticks_left_move <= 0) {
		// Reset ticks
		ticks_left_move = ticks_to_move;

		// Handle movement
		double compound_vel = (velocity / sqrt(2)) * 2;
		double mov_x = compound_vel;
		double mov_y = compound_vel;

		// Prioritize horizontal movement first, then vertical
		if (left) {
			if (up) {
				// Compound movement
				mov_x /= -2;
				mov_y = mov_x;
			} else if (down) {
				// Compound movement
				mov_x /= -2;
				mov_y = -mov_x;
			} else {
				// Mono movement
				facing = Player::FacingLeft;
				mov_x = -velocity;
				mov_y = 0;
			}
		} else if (right) {
			if (up) {
				// Compound movement
				mov_x /= 2;
				mov_y = -mov_x;
			} else if (down) {
				// Compound movement
				mov_x /= 2;
				mov_y = mov_x;
			} else {
				// Mono movement
				facing = Player::FacingRight;
				mov_x = velocity;
				mov_y = 0;
			}
		} else if (up) {
			// Mono movement
			facing = Player::FacingUp;
			mov_x = 0;
			mov_y = -velocity;
		} else if (down) {
			// Mono movement
			facing = Player::FacingDown;
			mov_x = 0;
			mov_y = velocity;
		}

		// Update NPC's coordinates
		x += mov_x;
		y += mov_y;
	}

	// Animation
	if (ticks_left_anim <= 0) {
		// Update the animation
		UpdateMovementSprite();
		ticks_left_anim = ticks_to_anim;
	}
}

/* ============================= Event Functions ==========================
*		Used according to the event triggered. (Do NOT define in header)
*			- Game ticks
*			- Mouse clicks
*			- Keyboard presses
*/
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	for (size_t i = 0; i < g->npc.size(); i++) {
		g->npc[i].HandleAI(g);
		if (g->npc[i].left || g->npc[i].right || g->npc[i].up || g->npc[i].down) {
			g->npc[i].HandleMovementLogic();
		} else {
			g->npc[i].HandleIdle();
		}
	}
}

static void Click(Game* g, int button, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Click
}

static void Press(Game* g, int kid, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Press
}


/* =========================== Register Function ==========================
*		The register function.
*		Registers the event functions to be triggered by the main loop.
*		May change the name and the event types to match the class purpose.
*
*		Call it once with the rest of init functions on main().
*/
int RegisterNPCEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
	for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
		if (t[i] == NULL && c[i] == NULL && p[i] == NULL) {
			t[i] = (TimerEventFunction)&Tick;		// Remove if not needed
			c[i] = (MouseEventFunction)&Click;		// Remove if not needed
			p[i] = (KeyboardEventFunction)&Press;	// Remove if not needed
			return i;
		}
	}
	return -1; // No slots
}