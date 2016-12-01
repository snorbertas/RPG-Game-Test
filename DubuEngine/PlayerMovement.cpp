#include "Game.h"
#include "PlayerMovement.h"

void UpdatePlayerMovementSprite(Player& pl) {
	pl.sprite_frame++;

	// Correct frames
	switch (pl.facing) {
	case Player::FacingUp:
		if (pl.sprite_frame < Player::FrameUp_0 ||
			pl.sprite_frame > Player::FrameUp_2) {
			pl.sprite_frame = 6;
		}
		break;
	case Player::FacingDown:
		if (pl.sprite_frame < Player::FrameDown_0 ||
			pl.sprite_frame > Player::FrameDown_2) {
			pl.sprite_frame = 0;
		}
		break;
	case Player::FacingLeft:
		if (pl.sprite_frame < Player::FrameHor_0 ||
			pl.sprite_frame > Player::FrameHor_2) {
			pl.sprite_frame = 3;
		}
		break;
	case Player::FacingRight:
		if (pl.sprite_frame < Player::FrameHor_0 ||
			pl.sprite_frame > Player::FrameHor_2) {
			pl.sprite_frame = 3;
		}
		break;
	}
}

bool HandlePlayerMovementKeyBinds(Game* g, int kid) {
	if (kid == g->keys.left_bind) {
		// Left bind
		g->pl.facing = Player::FacingLeft;
		g->keys.left = true;
		g->keys.right = false;
		return true;
	} else if (kid == g->keys.right_bind) {
		// Right bind
		g->pl.facing = Player::FacingRight;
		g->keys.right = true;
		g->keys.left = false;
		return true;
	} else if (kid == g->keys.up_bind) {
		// Up bind
		g->pl.facing = Player::FacingUp;
		g->keys.up = true;
		g->keys.down = false;
		return true;
	} else if (kid == g->keys.down_bind) {
		// Down bind
		g->pl.facing = Player::FacingDown;
		g->keys.down = true;
		g->keys.up = false;
		return true;
	}
	return false;
}

void HandlePlayerMovementLogic(Game* g) {
	if (g->keys.right || g->keys.left || g->keys.up || g->keys.down) {
		// There's movement input, update the tick counter
		g->pl.ticks_left--;

		// Time to mobilize
		if (g->pl.ticks_left <= 0) {
			// Reset ticks
			g->pl.ticks_left = g->pl.ticks_to_move;

			// Handle movement
			double compound_vel = g->pl.velocity * 2; // Not sure if too fast or not
			double mov_x = compound_vel;
			double mov_y = compound_vel;

			// Prioritize horizontal movement first, then vertical
			if (g->keys.left) {
				if (g->keys.up) {
					// Compound movement
					mov_x /= -2;
					mov_y = mov_x;
				} else if (g->keys.down) {
					// Compound movement
					mov_x /= -2;
					mov_y = -mov_x;
				} else {
					// Mono movement
					g->pl.facing = Player::FacingLeft;
					mov_x = -g->pl.velocity;
					mov_y = 0;
				}
			} else if (g->keys.right) {
				if (g->keys.up) {
					// Compound movement
					mov_x /= 2;
					mov_y = -mov_x;
				} else if (g->keys.down) {
					// Compound movement
					mov_x /= 2;
					mov_y = mov_x;
				} else {
					// Mono movement
					g->pl.facing = Player::FacingRight;
					mov_x = g->pl.velocity;
					mov_y = 0;
				}
			} else if (g->keys.up) {
				// Mono movement
				g->pl.facing = Player::FacingUp;
				mov_x = 0;
				mov_y = -g->pl.velocity;
			} else if (g->keys.down) {
				// Mono movement
				g->pl.facing = Player::FacingDown;
				mov_x = 0;
				mov_y = g->pl.velocity;
			}

			// Update player's coordinates
			// TODO: Insert collision here
			g->pl.x += mov_x;
			g->pl.y += mov_y;

			// Update the animation
			UpdatePlayerMovementSprite(g->pl);
		}
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
	HandlePlayerMovementLogic(g);
}

static void Click(Game* g, int button, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Click
}

static void Press(Game* g, int kid, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Press
	// Movement keys are handled with the rest of key binds in InputHandler.cpp
}


/* =========================== Register Function ==========================
*		The register function.
*		Registers the event functions to be triggered by the main loop.
*		May change the name and the event types to match the class purpose.
*
*		Call it once with the rest of init functions on main().
*/
int RegisterPlayerMovementEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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