#pragma once
#include <string>

class Player {
public:
	// This should correspond to the # in array for multiplayer
	int pID = -1;

	// Player name
	std::string name = "null";

	// Animation frames
	enum Frame {
		FrameDown_0, FrameDown_1, FrameDown_2,
		FrameHor_0, FrameHor_1, FrameHor_2,
		FrameUp_0, FrameUp_1, FrameUp_2,
		FrameIdleDown_0 = 0, FrameIdleDown_1 = 9,
		FrameIdleHor_0 = 4, FrameIdleHor_1 = 10,
		FrameIdleUp_0 = 7, FrameIdleUp_1 = 11
	};

	// Facing directions
	enum Facing {
		FacingUp, FacingDown, FacingLeft, FacingRight
	};

	// Bladder
	int pee_ammo = 0;

	// Location
	double x = 0;					// x/y coordinates
	double y = 0;					// Using decimals due to compound movement
	int facing = FacingUp;			// Direction player is facing

	// Size
	int w = 64;
	int h = 64;

	// Physics
	int velocity = 3;				// Pixels per movement
	int	ticks_to_move = 1;			// Ticks to move
	int ticks_left_move = ticks_to_move;	// Ticks left till next movement
	int ticks_to_anim = 5;			// Ticks to change sprite
	int ticks_left_anim = ticks_to_anim;	// Ticks left till sprite change

	// Visual/Animation
	int sprite_id = 0;				// Sprite start
	int sprite_frame = 0;			// Sprite offset

	// Multi-player
	bool connected = false;
};