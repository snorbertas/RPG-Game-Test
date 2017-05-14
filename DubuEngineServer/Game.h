#pragma once

#define MAX_PLAYERS 20

#include <winsock2.h>
#include "Player.h"
#include "Score.h"

class Game
{
public:
	Game();
	Player* players;					// Pointer to array of Players
	SOCKET ClientSocket[MAX_PLAYERS];	// Array of sockets for the clients
	bool SocketUsed[MAX_PLAYERS];		// Boolean to indicate the availability of a socket
	bool ready = false;					// Wether the server finished initializing/loading
	bool done = false;					// Wether the server is done running
	ScoreBoard score;					// Scoreboard
	int seed = 22;						// Seed for the map
	int map_trim = 40;					// Map trim

	// Timers
	int ticks_since_last_score_update = 0;
};