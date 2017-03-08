#pragma once
#include "Sprites.h"
#include "DEEvents.h"
#include <vector>
class Game;

// ========================== Sunlight class =============================
class Sunlight {
public:
	bool active = true;			// Wether sun is active or not (false means no shadows)
	int timer = -1;				// Timer for movement
	float brightness = 0.15;	// Brightness of shadows
	double position = -45;		// -90 degrees on left, 90 degrees on right
	int height = 20;			// 0px morning/evening, 20px mid-day
	void Update() {
		if (active) {
			position += 0.005;
			if (position > 90) position = -90;
		}
	}
};

// ============================ Cloud class ==============================
class Cloud {
public:
	Cloud(int ix, int iy, int ivel, int iflags) : x(ix), y(iy), vel(ivel), flags(iflags) {}
	int x;
	int y;
	float vel;
	int flags = 0;
};

// ============================ Cycle class ==============================
class DayNightCycle {
public:
	Sunlight sunlight;
	int timer = -1;
	bool night = false;
};

class Weather {
public:
	// Variables
	static const int MAX_CLOUDS = 20;
	static const int MAX_CLOUDS_MENU = 20;
	std::vector<Cloud> cloud;
	enum CloudMode { MODE_GAME, MODE_MENU };
	DayNightCycle cycle;

	// Functions
	Weather(Weather::CloudMode mode = MODE_MENU);
	void HandleClouds(Game* g, CloudMode = MODE_MENU);								// Handles clouds
	void RenderClouds(Game* g, SpriteStruct* sprites, CloudMode mode = MODE_MENU);	// Renders clouds
	void HandleDayNightCycle(Game* g);	// Handle day cycles
	void StartNight(Game* g);			// Starts a night cycle
	void StartDay(Game* g);				// Starts a day cycle
	void RenderDayNightCycle(Game* g);	// Render day cycles
};

int RegisterWeatherEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);