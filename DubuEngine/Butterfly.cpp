#include "Game.h"
#include "ScaledDraw.h"
#include "Collision.h"

void Butterfly::Draw(Game* g, SpriteStruct* sprites) {
	DrawImage(g, sprites->img_gfx[sprite_id + frame],
		x + g->camera.x,
		y - z + g->camera.y,
		facing);

	// Shadow according to sunlight
	if (g->weather.cycle.sunlight.active) {
		double shift_shadow_x = -64.0;
		shift_shadow_x *= ((double)g->weather.cycle.sunlight.position / 90.0);
		DrawRotatedShadow(g, sprites->img_gfx[sprite_id + frame],
			x + 16 + shift_shadow_x + g->camera.x,
			y + 64 + g->camera.y,
			0, facing, 0.2);
	}
}

void Butterfly::DecideDestination() {
	// Random destination
	x_destination = rand() % (Map::MAP_SIZE_X * Map::TILE_SIZE);
	y_destination = rand() % (Map::MAP_SIZE_Y * Map::TILE_SIZE);

	// Calculate distance required to travel
	double dist_x = x_destination - x;
	double dist_y = y_destination - y;

	// Unsign
	if (dist_x < 0) dist_x *= -1;
	if (dist_y < 0) dist_y *= -1;

	/* 0.1 is a tiny offset to prevent zero-division errors
	   in extremely rare cases where the butterfly's new
	   randomly picked destination goal is the exact same
	   pixel (out of 40,960,000 pixels).
	   
	   There's a 0.000002% chance of that happening, but
	   at least I'm preventing a random crash before it
	   happens */
	x_vel = (max_vel / (0.1 + dist_x + dist_y)) * dist_x;
	y_vel = (max_vel / (0.1 + dist_x + dist_y)) * dist_y;

	// Invert where needed
	if (x_destination < x) x_vel *= -1;
	if (y_destination < y) y_vel *= -1;
}

void Butterfly::Tick() {
	// Timer
	timer++;
	if (timer >= 4) {
		timer = 0;
		frame++;
		if (frame > 2) frame = 0;
	}

	// z (height) Movement
	z += z_vel;
	if (z_vel > 0) {
		z_vel += 0.5;
	} else {
		z_vel -= 0.5;
	}

	if (ascending) {
		if (z_vel < -4) z_vel = 0.1;
		ascending = false;
	} else {
		if (z_vel > 4) z_vel = -0.1;
		ascending = true;
	}

	// x/y movement
	x += x_vel;
	y += y_vel;

	// Facing
	if (x_vel > 0) facing = FacingRight;
	if (x_vel < 0) facing = FacingLeft;

	// AI
	CollisionBox cbox1(x, y, 32, 32);
	CollisionBox cbox2(x_destination, y_destination, 64, 64);
	if (collide(cbox1, cbox2)) {
		DecideDestination();
	}
}