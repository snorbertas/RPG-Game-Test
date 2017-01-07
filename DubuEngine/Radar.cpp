#include "Game.h"
#include "Radar.h"
#include "ScaledDraw.h"

/* ============================= Event Functions ==========================
*		Used according to the event triggered. (Do NOT define in header)
*			- Game ticks
*			- Mouse clicks
*			- Keyboard presses
*/

void CalculateBoneDistances(Game* g) {
	for (int i = 0; i < g->map.bone.size(); i++) {
		int bone_x = g->map.bone[i].x + (Map::TILE_SIZE / 2);
		int bone_y = g->map.bone[i].y + (Map::TILE_SIZE / 2);
		double player_x = g->pl.x + (Map::TILE_SIZE / 2);
		double player_y = g->pl.y + (Map::TILE_SIZE / 2);
		double distance_x = (bone_x - player_x) / Map::TILE_SIZE;
		double distance_y = (bone_y - player_y) / Map::TILE_SIZE;
		if (distance_x < 0) distance_x *= -1;
		if (distance_y < 0) distance_y *= -1;
		double distance = sqrt(pow(distance_x, 2) + pow(distance_y, 2));
		g->map.bone[i].distance = distance;
	}
}

void UpdateRadar(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Scan for closest bone
	MapObject* closest_bone = NULL;
	for (int i = 0; i < g->map.bone.size(); i++) {
		if (g->map.bone[i].distance <= g->radar.range) {
			if (closest_bone == NULL) {
				closest_bone = &g->map.bone[i];
			} else {
				if (closest_bone->distance > g->map.bone[i].distance) {
					closest_bone = &g->map.bone[i];
				}
			}
		}
	}

	// Update state
	if (closest_bone == NULL) {
		// Bone detected
		g->radar.bone_detected = false;
	} else {
		/* Set timer to -1 if this is the first tick a bone is found
		   This will further increment to 0 and will play a sound */
		if (g->radar.bone_detected == false) g->radar.timer = -1;

		// Bone detected
		g->radar.bone_detected = true;

		// Remember distance (for text rendering)
		g->radar.closest_distance = closest_bone->distance;

		// Timer
		g->radar.timer++;
		int anim_duration = 8 + closest_bone->distance * 6;
		if (g->radar.timer > anim_duration) {
			g->radar.timer = 0;
		}

		// Animation
		float animation_prog = (float)(g->radar.timer) / (float)(anim_duration);
		float new_scale = (0.2 * animation_prog);
		g->radar.scale = new_scale;
		g->radar.opacity = 1.0 - 0.5 * (closest_bone->distance / g->radar.range);

		// Sound
		if (g->radar.timer == 0) {
			float speed = 2.0 - (closest_bone->distance / g->radar.range);
			float volume = 0.5 + (speed - 1.5);
			al_play_sample(sample_sfx[0], volume, 0.0, speed, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}
}

void RenderRadar(Game* g, ALLEGRO_FONT** font, SpriteStruct* sprites) {
	// Interface sketch
	DrawRectangle(g, g->Interfaces[INTERFACE_RADAR].x - 10, g->Interfaces[INTERFACE_RADAR].y - 10, 63 + 20, 200, 100, 100, 100);
	DrawOutline(g, g->Interfaces[INTERFACE_RADAR].x - 10, g->Interfaces[INTERFACE_RADAR].y - 10, 63 + 20, 200, 0, 0, 0, 1);

	// Text
	float r_x = (float)g->s_x / (float)g->BWIDTH;
	float r_y = (float)g->s_y / (float)g->BHEIGHT;
	if (g->radar.bone_detected) {
		if (g->radar.closest_distance < 0.5) {
			DrawText(font[2], 10, 200, 10,
				g->Interfaces[INTERFACE_RADAR].x + 31, g->Interfaces[INTERFACE_RADAR].y + 40, ALLEGRO_ALIGN_CENTER,
				"Bone!!!");
		} else {
			DrawText(font[2], 200, 200, 10,
				g->Interfaces[INTERFACE_RADAR].x + 31, g->Interfaces[INTERFACE_RADAR].y + 40, ALLEGRO_ALIGN_CENTER,
				"Sniff...");
		}
	} else {
		DrawText(font[2], 50, 50, 50,
			g->Interfaces[INTERFACE_RADAR].x + 31, g->Interfaces[INTERFACE_RADAR].y + 40, ALLEGRO_ALIGN_CENTER,
			"No bones");
	}

	// Image
	/*DrawImage(g, sprites->img_interface[SPRITE_INTERFACE_RADAR],
		g->Interfaces[INTERFACE_RADAR].x, g->Interfaces[INTERFACE_RADAR].y, 0);*/

	int x_offset = (-63 * g->radar.scale) / 2;
	int y_offset = (-44 * g->radar.scale) / 2;
	DrawScaledImage(g, sprites->img_interface[SPRITE_INTERFACE_RADAR],
		g->Interfaces[INTERFACE_RADAR].x + x_offset,
		g->Interfaces[INTERFACE_RADAR].y + y_offset,
		63.0 * g->radar.scale, 44.0 * g->radar.scale, 0, g->radar.opacity);
}

static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	CalculateBoneDistances(g);
	UpdateRadar(g, sample_sfx);
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
int RegisterRadarEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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