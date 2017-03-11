#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string.h>
#include <iostream>
#include "Game.h"
#include "ScaledDraw.h"
#include "GameRenderer.h"
#include "Digging.h"
#include "Peeing.h"
#include "Drinking.h"
#include "Weather.h"

void RenderGame(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font) {
	// Render parallax
	RenderParallaxBackground(g, sprites);

	// Render Tiles
	g->map.RenderTiles(g, sprites);
	g->map.RenderBorders(g, sprites);

	// Render digging animation
	RenderDigging(g, sprites);

	// Render peeing animation
	RenderPeeing(g, sprites);

	// Render drinking animation
	RenderDrinking(g, sprites);

	// Render dirt particles
	RenderDirtParticles(g);

	// Sort and render objects
	g->map.SortPlayerObjects(g);
	g->map.RenderObjects(g, sprites);

	// Render butterflies
	g->map.RenderButterflies(g, sprites);

	// Render grid
	g->map.RenderGrid(g, sprites);

	// Render sniff info (debug/temp)
	/*if (g->debug.grid) {
		for (size_t i = 0; i < g->map.Bones.size(); i++) {
			DrawOutline(g, g->map.Bones[i].x + g->camera.x, g->map.Bones[i].y + g->camera.y,
				Map::TILE_SIZE, Map::TILE_SIZE, 255, 255, 255, 3);
			float r_x = (float)g->s_x / (float)g->BWIDTH;
			float r_y = (float)g->s_y / (float)g->BHEIGHT;
			DrawText(font[4], 255, 255, 255, g->map.Bones[i].x + g->camera.x + 4, g->map.Bones[i].y + g->camera.y,
				ALLEGRO_ALIGN_LEFT, "Bone: #%i", i);
			DrawText(font[4], 255, 255, 255, g->map.Bones[i].x + g->camera.x + 4, g->map.Bones[i].y + g->camera.y + 12,
				ALLEGRO_ALIGN_LEFT, "%f", g->map.Bones[i].Distance);
		}
	}*/

	// Render zone info (debug/temp)
	if (g->debug.grid) {
		for (int x = 0; x < Map::MAP_SIZE_X; x++) {
			int x_real = (x + 1) * Map::TILE_SIZE;
			if (x_real >= -g->camera.x && x_real <= -g->camera.x + g->BWIDTH - (Map::TILE_SIZE * -1)) {
				for (int y = 0; y < Map::MAP_SIZE_X; y++) {
					int y_real = (y + 1) * Map::TILE_SIZE;
					if (y_real >= -g->camera.y && y_real <= -g->camera.y + g->BHEIGHT - (Map::TILE_SIZE * -1)) {
						float r_x = (float)g->s_x / (float)g->BWIDTH;
						float r_y = (float)g->s_y / (float)g->BHEIGHT;
						if (g->map.zone[x][y].Owner > -2) {
							DrawText(font[0], 255, 255, 0,
								(x * Map::TILE_SIZE) + g->camera.x + (Map::TILE_SIZE / 2),
								(y * Map::TILE_SIZE) + g->camera.y + (5),
								ALLEGRO_ALIGN_CENTER, "%i", g->map.zone[x][y].Owner);
						}

						int re = 0;
						int gr = 0;
						int bl = 0;
						if (g->map.zone[x][y].BoneSweeperReal == Zone::BoneSweeper::Tunnel) {
							re = 255;
							gr = 255;
							bl = 255;
						}
						if (g->map.zone[x][y].BoneSweeperReal >= Zone::BoneSweeper::TouchingOne) {
							re = (255 * ((double)(g->map.zone[x][y].BoneSweeperReal - Zone::BoneSweeper::TouchingOne) / 7.0)) + 31;
						}
						if (g->map.zone[x][y].BoneSweeperReal > Zone::BoneSweeper::None) {
							DrawText(font[0], 0, 0, 0,
								(x * Map::TILE_SIZE) + g->camera.x + (Map::TILE_SIZE / 2),
								(y * Map::TILE_SIZE) + g->camera.y + (30),
								ALLEGRO_ALIGN_CENTER, "%i", g->map.zone[x][y].BoneSweeperReal);
						}
					}
				}
			}
		}
	}

	// Render clouds
	g->weather.RenderClouds(g, sprites);

	// Render day/night cycle
	g->weather.RenderDayNightCycle(g);
}

void RenderPlayer(Game* g, Player& pl, SpriteStruct* sprites) {
	// Render a player
	int flag = 0;
	if (pl.facing == 3) flag = 1;
	DrawImage(g,
		sprites->img_body[pl.sprite_id + pl.sprite_frame],
		pl.x + g->camera.x,
		pl.y + g->camera.y, flag);
}

void RenderPlayerShadow(Game* g, Player& pl, SpriteStruct* sprites) {
	// Render a player's shadow
	float opacity = g->weather.cycle.sunlight.brightness;
	DrawImage(g, sprites->img_gfx[SPRITE_SHADOW_PLAYER],
		pl.x + g->camera.x,
		pl.y + 5 + g->camera.y,
		0, opacity);
}

void RenderParallaxBackground(Game* g, SpriteStruct* sprites) {
	int x_offset = 0;
	while ((x_offset + g->camera.x * 0.4 - 1800) < -1800) {
		x_offset += 1800;
	}
	int x = x_offset + g->camera.x * 0.4;
	int y = -150 + g->camera.y * 0.2;
	if (y > 0) y = 0;
	DrawImage(g, sprites->img_background[BACKGROUND_SKY], x - 1800, y, 0);
	DrawImage(g, sprites->img_background[BACKGROUND_SKY], x, y, 0);
}