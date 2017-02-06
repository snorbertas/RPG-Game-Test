#pragma once
#include "DEEvents.h"

struct Debug {
	bool grid = false;
	bool renderChange = false;
	bool forestChange = false;
};

int RegisterDebugEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
