#include "Game.h"
#include "PlayerMovement.h"
#include "Collision.h"
#include "GameHandler.h"
#include "Digging.h"
#include "Peeing.h"
#include "Drinking.h"

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

void QueueMovementPacket(Game* g) {
	PacketPlayerState* packet = new PacketPlayerState(PACKET_TYPE_PLAYER_STATE);
	packet->facing = g->pl.facing;
	packet->x = g->pl.x;
	packet->y = g->pl.y;
	packet->frame = g->pl.sprite_frame;
	AddPacketToQueue(g, packet);
}

void HandlePlayerIdle(Game* g) {
	// Update anim counter
	g->pl.ticks_left_anim--;		
		
	// Animation
	if (g->pl.ticks_left_anim <= 0) {
		// Update the animation
		if (g->pl.digging) {
			if (g->pl.sprite_frame != Player::FrameDig_0) {
				g->pl.sprite_frame = Player::FrameDig_0;
			} else {
				g->pl.sprite_frame = Player::FrameDig_1;
			}
		} else if (g->pl.peeing) {
			g->pl.sprite_frame = Player::FramePee;
		} else if (g->pl.drinking) {
			g->pl.sprite_frame = Player::FrameDrink;
		} else if (g->pl.facing == Player::FacingDown) {
			if (g->pl.sprite_frame != Player::FrameIdleDown_0) {
				g->pl.sprite_frame = Player::FrameIdleDown_0;
			} else {
				g->pl.sprite_frame = Player::FrameIdleDown_1;
			}
		} else if (g->pl.facing == Player::FacingLeft ||
					g->pl.facing == Player::FacingRight) {
			if (g->pl.sprite_frame != Player::FrameIdleHor_0) {
				g->pl.sprite_frame = Player::FrameIdleHor_0;
			} else {
				g->pl.sprite_frame = Player::FrameIdleHor_1;
			}
		} else if (g->pl.facing == Player::FacingUp) {
			if (g->pl.sprite_frame != Player::FrameIdleUp_0) {
				g->pl.sprite_frame = Player::FrameIdleUp_0;
			} else {
				g->pl.sprite_frame = Player::FrameIdleUp_1;
			}
		}

		// Reset anim ticks
		g->pl.ticks_left_anim = g->pl.ticks_to_anim * 2;

		// Multiplayer
		if (g->connected) {
			if (g->pl.ticks_left_anim == g->pl.ticks_to_anim ||
				g->pl.ticks_left_move == g->pl.ticks_to_move) {
				// Add a packet to queue concerning our current state
				QueueMovementPacket(g);
			}
		}
	}
}

void HandlePlayerMovementLogic(Game* g) {
	// There's movement input, update the tick counters
	g->pl.ticks_left_move--;
	g->pl.ticks_left_anim--;

	// Time to mobilize
	if (g->pl.ticks_left_move <= 0) {
		// Reset ticks
		g->pl.ticks_left_move = g->pl.ticks_to_move;

		// Handle movement
		double compound_vel = (g->pl.velocity / sqrt(2)) * 2;
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
		if (!PlayerMoveCollides(g->pl, g->map, mov_x, 0)) {
			g->pl.x += mov_x;
		} else {
			// Fix facing direction since we cant move in mov_x direction
			if (mov_y > 0) g->pl.facing = Player::FacingDown;
			if (mov_y < 0) g->pl.facing = Player::FacingUp;
		}
		if (!PlayerMoveCollides(g->pl, g->map, 0, mov_y)) {
			g->pl.y += mov_y;
		} else {
			// Fix facing direction since we cant move in mov_y direction
			if (mov_x > 0) g->pl.facing = Player::FacingRight;
			if (mov_x < 0) g->pl.facing = Player::FacingLeft;
		}

		using EType = MapObjectInfo::EMapObjectType;
		// Check collision with idle grass for animation
		for (auto& obj : g->map.GrassObjects) {
			if (obj->SpriteId == EObjectSpriteGrass_0) {
				// Check collision
				CollisionBox grass(obj->x + 8, obj->y + 8, 56, 56);
				CollisionBox player(g->pl.x, g->pl.y, g->pl.w, g->pl.h);

				// Adjust the player box
				player.x += 28;
				player.y += 56;
				player.w = 8;
				player.h = 8;

				// Animate if colliding
				if (collide(grass, player)) {
					obj->SpriteId = EObjectSpriteGrass_1;
					obj->AnimTimer = SecondsToTicks(0.3);
				}
			}
		}
	}

	// Animation
	if (g->pl.ticks_left_anim <= 0) {
		// Update the animation
		UpdatePlayerMovementSprite(g->pl);
		g->pl.ticks_left_anim = g->pl.ticks_to_anim;
	}

	// Multiplayer (if either timer got reset)
	if (g->connected) {
		if (g->pl.ticks_left_anim == g->pl.ticks_to_anim ||
			g->pl.ticks_left_move == g->pl.ticks_to_move) {
			// Add a packet to queue concerning our current state
			QueueMovementPacket(g);
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
	if (g->pl.digging) {
		HandleDigging(g, &g->pl);
		HandlePlayerIdle(g);
	} else if (g->pl.peeing) {
		HandlePeeing(g, &g->pl);
		HandlePlayerIdle(g);
	} else if (g->pl.drinking) {
		HandleDrinking(g, &g->pl);
		HandlePlayerIdle(g);
	} else {
		if (g->keys.right || g->keys.left || g->keys.up || g->keys.down) {
			HandlePlayerMovementLogic(g);
		} else {
			HandlePlayerIdle(g);
		}
	}
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