#pragma once
#include "DEEvents.h"

struct Debug {
	bool grid = false;
};

int RegisterDebugEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
