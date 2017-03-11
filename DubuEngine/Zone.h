#pragma once

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

	// For bone sweeper
	enum BoneSweeper { None, Tunnel, Mine,
		TouchingOne, TouchingTwo, TouchingThree, TouchingFour, TouchingFive, TouchingSix, TouchingSeven, TouchingEight };

	// Real value for this zone
	BoneSweeper BoneSweeperReal = None;

	// The known to player value for this zone
	BoneSweeper BoneSweeperKnown = None;
	// ^ dont need this if we can use map objects to identify this info
	
	void Mark(int owner) {
		Owner = owner;
	}
};