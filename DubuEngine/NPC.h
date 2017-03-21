#pragma once
#include "DEEvents.h"
#include "Player.h"
#include "Pathing.h"

class Game;

// For now keeping most variables simillar to Player class
class NPC {
public:
	// Types
	enum Type { None, Monster };
	Type type = None;

	// Constructor
	NPC(Type t, int spawn_x, int spawn_y, int new_id) : type(t), x(spawn_x), y(spawn_y), id(new_id) {
		if (type == Monster) sprite_id = SPRITE_BODY_MONSTER;
	}

	// NPC ID
	int id = -1;

	// Location
	double x = 0;					// x/y coordinates
	double y = 0;					// Using decimals due to compound movement
	int facing = Player::FacingUp;	// Direction NPC is facing

	// Convert real coordinates to map coordinates
	int MapX() { return round(x / Map::TILE_SIZE); }
	int MapY() { return round(y / Map::TILE_SIZE); }
	Node GetNodePosition() { return Node(MapX(), MapY()); }

	// Size
	int w = 64;
	int h = 64;

	/* Physics */
	// Bases
	int base_velocity = 4;			// Base velocity
	int base_ticks_to_anim = 4;		// Base ticks for animation

	// Real values
	int velocity = base_velocity;	// Pixels per movement
	double sprint_multiplier = 1.5;	// Multiplier for velocity from base movement speed
	int	ticks_to_move = 1;			// Ticks to move
	int ticks_left_move = ticks_to_move;	// Ticks left till next movement
	int ticks_to_anim = base_ticks_to_anim;	// Ticks to change sprite
	int ticks_left_anim = ticks_to_anim;	// Ticks left till sprite change

	// Visual/Animation
	int sprite_id = 0;				// Sprite start
	int sprite_frame = 0;			// Sprite offset

	// AI Control
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool idle = true;
	int idle_timer = 0;

	// Radii and idle duration
	int roam_radius = 30;				// Radius within the NPC can roam (map coordinates)
	int aggression_radius = 640;		// Radius within the NPC can detect a player (real coordinates)
	float idle_min_duration = 2;		// Minimum seconds idle duration
	int idle_extra_rand_duration = 2;	// Extra seconds idle duration

	// Pathing
	Path path;
	Node PickRandomDestination(Map& map);

	// Handles the AI for NPC
	void HandleAI(Game* g);
	void HandleMovementLogic();
	void HandleIdle();
	void UpdateMovementSprite();
	bool PlayerInRange(Player& p);
};

int RegisterNPCEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);