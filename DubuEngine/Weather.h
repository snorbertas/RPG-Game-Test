#pragma once
#include "Sprites.h"
#include "DEEvents.h"
#include <vector>
class Game;

// ============================ Cloud struct ==============================
class Cloud {
public:
	Cloud(int ix, int iy, int ivel, int iflags) : x(ix), y(iy), vel(ivel), flags(iflags) {}
	int x;
	int y;
	int vel;
	int flags = 0;
};

class Weather {
public:
	// Variables
	static const int MAX_CLOUDS = 20;
	static const int MAX_CLOUDS_MENU = 20;
	std::vector<Cloud> cloud;
	enum CloudMode { MODE_GAME, MODE_MENU };

	// Functions
	Weather(Weather::CloudMode mode = MODE_MENU);
	void HandleClouds(Game* g, CloudMode = MODE_MENU);								// Handles clouds
	void RenderClouds(Game* g, SpriteStruct* sprites, CloudMode mode = MODE_MENU);	// Renders clouds
};

int RegisterWeatherEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);