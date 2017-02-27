#include "Weather.h"
#include "ScaledDraw.h"
#include "Map.h"
#include "Game.h"
#include <algorithm>

Weather::Weather(CloudMode mode) {
	// Clean clouds
	cloud.empty();

	if (mode == CloudMode::MODE_GAME) { // In-game
		// Generate new ones
		for (int i = 0; i < MAX_CLOUDS; i++) {
			// Initialized random clouds
			Cloud new_cloud = Cloud(rand() % (Map::MAP_SIZE_X * Map::TILE_SIZE),
				rand() % (Map::MAP_SIZE_Y * Map::TILE_SIZE),
				1 + rand() % 2,
				rand() % 2);
			cloud.push_back(new_cloud);
		}
	} else if (mode == CloudMode::MODE_MENU) { // Menu
		// Generate new ones
		for (int i = 0; i < MAX_CLOUDS_MENU; i++) {
			Cloud new_cloud = Cloud(-1280 + rand() % 2560,
				((Map::MAP_SIZE_Y * Map::TILE_SIZE) - 720) + rand() % 1440,
				1 + rand() % 2,
				rand() % 2);
			cloud.push_back(new_cloud);
		}
	}

	// Sort them so they render in order
	sort(cloud.begin(), cloud.end(), [](const auto& a, const auto& b) { return a.y < b.y; });
}

void Weather::HandleClouds(Game* g, CloudMode mode) {
	for (size_t i = 0; i < cloud.size(); i++) {
		// Move it
		cloud[i].x += cloud[i].vel;

		// Reset
		if (mode == CloudMode::MODE_GAME) {
			if (cloud[i].x > (Map::MAP_SIZE_X * Map::TILE_SIZE) + g->BWIDTH) {
				cloud[i].x = -g->BWIDTH;
				cloud[i].y = rand() % Map::MAP_SIZE_Y * Map::TILE_SIZE;

				// Sort them so they render in order
				sort(cloud.begin(), cloud.end(), [](const auto& a, const auto& b) { return a.y < b.y; });
			}
		} else if (mode == CloudMode::MODE_MENU) {
			if (cloud[i].x > g->BWIDTH * 3) {
				cloud[i].x = -g->BWIDTH;
				cloud[i].y = ((Map::MAP_SIZE_Y * Map::TILE_SIZE) - (g->BHEIGHT / 2)) + rand() % g->BHEIGHT * 1.5;
			}

			// Sort them so they render in order
			sort(cloud.begin(), cloud.end(), [](const auto& a, const auto& b) { return a.y < b.y; });
		}
	}
}

void Weather::RenderClouds(Game* g, SpriteStruct* sprites, CloudMode mode) {
	float opacity = 0.9;
	if (mode == CloudMode::MODE_MENU) opacity = 0.5;
	for (size_t i = 0; i < cloud.size(); i++) {
		// Draw cloud
		DrawImage(g, sprites->img_background[BACKGROUND_CLOUD],
			cloud[i].x + g->camera.x,
			cloud[i].y + g->camera.y,
			cloud[i].flags, opacity);

		// Draw shadow
		DrawImage(g, sprites->img_background[BACKGROUND_CLOUD_SHADOW],
			cloud[i].x + g->camera.x - 200,
			cloud[i].y + g->camera.y + 550,
			cloud[i].flags, opacity);
	}
}

static void Tick(Game* g, ALLEGRO_SAMPLE** sample_sfx) {
	// Tick
	g->weather.HandleClouds(g, Weather::CloudMode::MODE_GAME);
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