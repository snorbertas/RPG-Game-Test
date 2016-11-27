#include "Game.h"
#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {

}

void SetCamera(Game* g, int x, int y, int mode) {
	g->camera.mode = mode;
	g->camera.x = -x;
	g->camera.y = -y;
	MoveCamera(g, x, y);
}

void MoveCamera(Game* g, int to_x, int to_y, int x_vel, int y_vel) {
	g->camera.mode = CAMERA_MODE_CUTSCENE;
	g->camera.move_to_x = -to_x;
	g->camera.move_to_y = -to_y;
	g->camera.x_vel = x_vel;
	g->camera.y_vel = y_vel;
}

void HandleCamera(Game* g) {
	if (g->camera.mode == CAMERA_MODE_LOCKED) {
		// Set camera's offset depending on player's position
		/*g->camera.x = -(p->x - (g->BWIDTH / 2));
		g->camera.y = -(p->y - (g->BHEIGHT / 2));
		g->camera.x = -(p->x - (g->BWIDTH / 2));
		g->camera.y = -(p->y - (g->BHEIGHT / 2));*/
	} else if (g->camera.mode == CAMERA_MODE_CUTSCENE) {
		if (g->camera.x < g->camera.move_to_x) {
			g->camera.x += g->camera.x_vel;
			if (g->camera.x > g->camera.move_to_x) g->camera.x = g->camera.move_to_x;
		} else if (g->camera.x > g->camera.move_to_x) {
			g->camera.x -= g->camera.x_vel;
			if (g->camera.x < g->camera.move_to_x) g->camera.x = g->camera.move_to_x;
		}
		if (g->camera.y < g->camera.move_to_y) {
			g->camera.y += g->camera.y_vel;
			if (g->camera.y > g->camera.move_to_y) g->camera.y = g->camera.move_to_y;
		} else if (g->camera.y > g->camera.move_to_y) {
			g->camera.y -= g->camera.x_vel;
			if (g->camera.y < g->camera.move_to_y) g->camera.y = g->camera.move_to_y;
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
	HandleCamera(g);
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
int RegisterCameraEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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