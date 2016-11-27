#pragma once
#include "Game.h"

/* ================================ NewGame ===============================
 *		This function should be called to reset all the game variables.
 *
 *		Call on death events, new game, etc.
 */
	void NewGame(Game* g);				// Game pointer


/* ============================== HandleGame ==============================
 *		Handles a game-tick. This function's job is to call all handling
 *		functions. Physics, animations, movement of any kind, etc.
 *
 *		Called from the main function.
 */
	void HandleGame(Game* g,			// Game pointer
		ALLEGRO_SAMPLE** sample_sfx);	// Pointer to array of sound effects


/* ============================ HandleGameInput ===========================
 *		Handles tick-relative input.
 *
 *		Called from HandleGame function.
 */
	void HandleGameInput(Game* g,
		ALLEGRO_SAMPLE** sample_sfx);


/* =========================== HandleGamePackets ==========================
 *		Handles socket/client-specific jobs.
 *
 *		Called from HandleGame function.
 */
	void HandleGamePackets(Game* g);

	
/* ============================ SecondsToTicks ============================
 *		Coverts seconds into a number of game ticks.
 *		Returns the number of game ticks.
 *
 *		Used when desired.
 */
	int SecondsToTicks(float seconds);		// Seconds to convert into ticks


/* ================================ collide ===============================
 *		Returns true if collision occurs between two objects or shapes.
 *
 *		Used to check collision.
 */
	// Rectangle collision
	bool collide(int o1x, int o1y,	// Object 1 x, y coordinates
		int o1w, int o1h,			// Object 1 width and height
		int o2x, int o2y,			// Object 2 x, y coordinates
		int o2w, int o2h);			// Object 2 width and height

	// Circle collision
	bool collide(int o1x, int o1y,	// Object 1 x, y coordinates
		int o1r,					// Object 1 radius
		int o2x, int o2y,			// Object 2 x, y coordinates
		int o2r);					// Object 2 radius

/* ============================== ShowMessage =============================
 *		Displays the message interface with specified text.
 *
 *		Used when desired.
 */
	void ShowMessage(Game* g,
		const char* msg1,					// First (top) part of message
		const char* msg2);					// Second (bottom) part of message


/* ================================= Fade =================================
 *		Starts a fade-in or fade-out
 *
 *		Used when desired.
 */
	void Fade(Game* g,
		int oi);							// 0 = Fade-in, 1 = Fade-out

/* =============================== MouseIsOn ==============================
 *		Returns true if mouse is on specified rectangle.
 *
 *		Used when desired.
 */
	bool MouseIsOn(Game* g,
		int x, int y,						// x, y coordinates of rectangle
		int w, int h);						// Width and height of rectangle


/* ============================ AddPacketToQueue ==========================
 *		Adds a packet to the packet queue.
 *		Returns the slot ID in queue.
 *		Returns -1 if queue is full.
 *
 *		Used when desired.
 */
	int AddPacketToQueue(Game* g,			// Game pointer
		Packet* p);							// Packet to add