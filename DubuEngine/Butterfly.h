#pragma once
#include "DEDefinitions.h"

class Game;
struct SpriteStruct;

// Note: A butterfly is just a GFX effect
class Butterfly {
public:
	enum FacingDir { FacingLeft, FacingRight };

	// Constructor
	Butterfly(int x, int y) : x(x), y(y) {
		DecideDestination();

		// Choose a butterfly sprite (colour/type)
		sprite_id = SPRITE_BUTTERFLY_O + ((rand() % 5) * 3);

		// Choose random velocity (2.5 to 4.5)
		max_vel = 2.5 + ((rand() % 200) / 100.0);

		// Random timer and frame (so each butterfly looks unique)
		timer = rand() % 5;
		frame = rand() % 3;

		// Random z velocity
		z_vel = -4 + ((rand() % 800) / 100.0);
	};

	// Variables
	int sprite_id = 11;
	int frame = 0;
	FacingDir facing = FacingLeft;
	double x;
	double y;
	double z = 20;
	int timer = 0;
	double max_vel = 3.0;
	double x_vel = 5;
	double y_vel = 5;
	double z_vel = 0.1;
	bool ascending = false;

	// AI
	int x_destination;
	int y_destination;

	void Draw(Game* g, SpriteStruct* sprites);
	void DecideDestination();
	void Tick();
};