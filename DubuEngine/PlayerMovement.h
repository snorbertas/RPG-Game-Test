#pragma once
#include "DEEvents.h"

int RegisterPlayerMovementEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);

/* ==================== HandlePlayerMovementKeyBinds ======================
 *		Handles the up/down/left/right key binds for movement.
 *		Returns true if a one of the key binds were detected and handled.
 *		Return false if there was no player movement input.
 *
 *		Called from InputHandler.cpp where key binds are handled.
 */
	bool HandlePlayerMovementKeyBinds(Game* g, int kid);