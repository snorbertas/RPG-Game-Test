#pragma once
#include "Sprites.h"
#include "DEEvents.h"
#include <vector>
class Game;

// ============================ Cloud class ==============================
class Cloud {
public:
	Cloud(int ix, int iy, int ivel, int iflags) : x(ix), y(iy), vel(ivel), flags(iflags) {}
	int x;
	int y;
	int vel;
	int flags = 0;
};

// ============================ Cycle class ==============================
class DayNightCycle {
public:
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