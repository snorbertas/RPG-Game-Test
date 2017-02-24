#include "Weather.h"
#include "ScaledDraw.h"
#include "Map.h"
#include "Game.h"
#include <algorithm>

Weather::Weather() {
	// Clean clouds
	cloud.empty();

	// Generate new ones
	for (int i = 0; i < MAX_CLOUDS; i++) {
		// Initialized random clouds
		Cloud new_cloud = Cloud(rand() % Map::MAP_SIZE_X * Map::TILE_SIZE,
			rand() % Map::MAP_SIZE_Y * Map::TILE_SIZE,
			1 + rand() % 2,
			rand() % 2);
		cloud.push_back(new_cloud);
	}

	// Sort them so they render in order
	sort(cloud.begin(), cloud.end(), [](const auto& a, const auto& b) { return a.y < b.y; });
}

void Weather::HandleClouds(Game* g) {
	for (int i = 0; i < MAX_CLOUDS; i++) {
		// Move it
		cloud[i].x += cloud[i].vel;

		// Reset
		if (cloud[i].x > (Map::MAP_SIZE_X * Map::TILE_SIZE) + g->BWIDTH) {
			cloud[i].x = -g->BWIDTH;
			cloud[i].y = rand() % Map::MAP_SIZE_Y * Map::TILE_SIZE;
		}
	}
}

void Weather::RenderClouds(Game* g, SpriteStruct* sprites) {
	for (int i = 0; i < MAX_CLOUDS; i++) {
		// Draw cloud
		DrawImage(g, sprites->img_background[BACKGROUND_CLOUD],
			cloud[i].x + g->camera.x,
			cloud[i].y + g->camera.y,
			cloud[i].flags, 0.9);

		// Draw shadow
		DrawImage(g, sprites->img_background[BACKGROUND_CLOUD_SHADOW],
			cloud[i].x + g->camera.x - 200,
			cloud[i].y + g->camera.y + 550,
			cloud[i].flags, 0.9);
	}
}

static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	g->weather.HandleClouds(g);
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
int RegisterWeatherEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p) {
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