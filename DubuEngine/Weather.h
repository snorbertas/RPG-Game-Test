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
	std::vector<Cloud> cloud;

	// Functions
	Weather();
	void HandleClouds(Game* g);							// Handles clouds
	void RenderClouds(Game* g, SpriteStruct* sprites);	// Renders clouds
};

int RegisterWeatherEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);