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
#include "GameHandler.h"
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

						// BoneSweeper
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

		if (g->npc.size() > 0) {
			if (g->npc[0].path.successful) {
				float r_x = (float)g->s_x / (float)g->BWIDTH;
				float r_y = (float)g->s_y / (float)g->BHEIGHT;
				for (size_t i = 0; i < g->npc[0].path.node.size(); i++) {
					DrawText(font[0], 255, 255, 255,
						g->npc[0].path.node[i].first + g->camera.x,
						g->npc[0].path.node[i].second + g->camera.y - 17,
						ALLEGRO_ALIGN_CENTER, "%i", i);
				}
			}
		}
	}


	// Render territory information
	RenderTerritoryInfo(g, sprites, font);

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

void RenderNPC(Game* g, NPC& npc, SpriteStruct* sprites) {
	// Render an NPC
	int flag = 0;
	if (npc.facing == 3) flag = 1;
	DrawImage(g,
		sprites->img_body[npc.sprite_id + npc.sprite_frame],
		npc.x + g->camera.x,
		npc.y + g->camera.y, flag);
}

void RenderNPCShadow(Game* g, NPC& npc, SpriteStruct* sprites) {
	// Render an NPC's shadow
	float opacity = g->weather.cycle.sunlight.brightness;
	DrawImage(g, sprites->img_gfx[SPRITE_SHADOW_PLAYER],
		npc.x + g->camera.x,
		npc.y + 5 + g->camera.y,
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