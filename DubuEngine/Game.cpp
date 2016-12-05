#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include "Game.h"
#include "GameHandler.h"

Game::Game(){
	// Initiate multi players array
	for (int i = 0; i < MAX_PLAYERS; i++) {
		// Let it know what index it is
		Players[i].pID = i;
	}
}