#include <fstream>
#include "Game.h"

Game::Game() {
	this->ready = false;
	this->done = false;
	this->players = new Player[MAX_PLAYERS];
	for (int i = 0; i < MAX_PLAYERS; i++) {
		SocketUsed[i] = false;
	}
}