#pragma once
#include <allegro5/allegro_font.h>

class Map;

/*
	A zone is a single tile that holds some data about
	it for different game modes such as:
		- Ownership of that square
		- Tunnels
		- Mines
		- Number of mines surrounded the square
*/

class Zone {
public:
	// For territories
	// -2 = None, -1 = Local Player, 0 and higher are indexes for multiplayer
	int Owner = -2;

	// For bone sweeper (really important that Tunnel = 0, TouchingOne = 1 up to TouchingEight = 8)
	enum BoneSweeper { None = -2, Mine, Tunnel = 0,
		TouchingOne = 1, TouchingTwo, TouchingThree, TouchingFour, TouchingFive, TouchingSix, TouchingSeven, TouchingEight };

	// Real value for this zone
	BoneSweeper BoneSweeperReal = None;

	// The known to player value for this zone
	BoneSweeper BoneSweeperKnown = None;
	// ^ dont need this if we can use map objects to identify this info

	// Marks this territory with the owner ID specified
	void Mark(int owner) {
		Owner = owner;
	}
};

class Territory {
public:
	// Constructor
	Territory(int new_owner) : owner(new_owner) {}

	// Target owner
	int owner = -2;

	// Holds a collection of x/y indexes to zones that are part of this territory
	std::vector<std::pair<int, int>> zone;

	// Recalculates the territory
	void Recalculate(Map* map, int new_owner = -2);
};

// Renders territory information
void RenderTerritoryInfo(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font);