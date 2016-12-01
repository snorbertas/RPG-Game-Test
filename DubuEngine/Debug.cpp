#include "Game.h"
#include "Debug.h"

/* ============================= Event Functions ==========================
*		Used according to the event triggered. (Do NOT define in header)
*			- Game ticks
*			- Mouse clicks
*			- Keyboard presses
*/
static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	if (g->pos_x < 50) {
		g->camera.x += 20;
		g->camera.mode = CAMERA_MODE_CUTSCENE;
	}
	if (g->pos_x > g->BWIDTH - 50) {
		g->camera.x -= 20;
		g->camera.mode = CAMERA_MODE_CUTSCENE;
	}
	if (g->pos_y < 50) {
		g->camera.y += 20;
		g->camera.mode = CAMERA_MODE_CUTSCENE;
	}
	if (g->pos_y > g->BHEIGHT - 50) {
		g->camera.y -= 20;
		g->camera.mode = CAMERA_MODE_CUTSCENE;
	}
}

static void Click(Game* g, int button, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Click
}

static void Press(Game* g, int kid, bool release, ALLEGRO_SAMPLE** sample_sfx) {
	// Press
	// Register different keys to help change/debug variables live
	if (!release) {
		if (kid == ALLEGRO_KEY_F1) {
			// PANIC BUTTON
			g->BWIDTH = 1280;
			g->BHEIGHT = 720;
			g->camera.x = 0;
			g->camera.y = 0;
		}
		if (kid >= ALLEGRO_KEY_PAD_0 && kid <= ALLEGRO_KEY_PAD_9) {
			g->map.seed = kid - ALLEGRO_KEY_PAD_0;
		} else if (kid >= ALLEGRO_KEY_A && kid <= ALLEGRO_KEY_Z) {
			g->map.GenerateRandom(kid - ALLEGRO_KEY_A);
		} else if (kid == ALLEGRO_KEY_F5) {
			g->pl.ticks_to_move--;
		} else if (kid == ALLEGRO_KEY_F6) {
			g->pl.ticks_to_move++;
		} else if (kid == ALLEGRO_KEY_F7) {
			g->pl.velocity--;
		} else if (kid == ALLEGRO_KEY_F8) {
			g->pl.velocity++;
		} else if (kid == ALLEGRO_KEY_F9) {
			g->pl.ticks_to_anim--;
		} else if (kid == ALLEGRO_KEY_F10) {
			g->pl.ticks_to_anim++;
		}
	}
}


/* =========================== Register Function ==========================
*		The register function.
*		Registers the event functions to be triggered by the main loop.
*		May change the name and the event types to match the class purpose.
*
*		Call it once with the rest of init functions on main().
*/
int RegisterDebugEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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