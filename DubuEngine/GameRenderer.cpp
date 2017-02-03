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

void RenderGame(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font){
	// Render Tiles
	g->map.RenderTiles(g, sprites);

	// Render digging tiles
	RenderDigging(g, sprites);

	// Sort and render objects
	g->map.SortPlayerObjects(g);
	g->map.RenderObjects(g, sprites);

	// Render grid
	g->map.RenderGrid(g, sprites);

	// Render sniff info (debug/temp)
	if (g->debug.grid) {
		for (int i = 0; i < g->map.Bones.size(); i++) {
			DrawOutline(g, g->map.Bones[i].x + g->camera.x, g->map.Bones[i].y + g->camera.y,
				Map::TILE_SIZE, Map::TILE_SIZE, 255, 255, 255, 3);
			float r_x = (float)g->s_x / (float)g->BWIDTH;
			float r_y = (float)g->s_y / (float)g->BHEIGHT;
			DrawText(font[4], 255, 255, 255, g->map.Bones[i].x + g->camera.x + 4, g->map.Bones[i].y + g->camera.y,
				ALLEGRO_ALIGN_LEFT, "Bone: #%i", i);
			DrawText(font[4], 255, 255, 255, g->map.Bones[i].x + g->camera.x + 4, g->map.Bones[i].y + g->camera.y + 12,
				ALLEGRO_ALIGN_LEFT, "%f", g->map.Bones[i].Distance);
		}
	}

}

void RenderPlayer(Game* g, Player& pl, SpriteStruct* sprites) {
	// Render player test (Maybe put this in a separate file?)
	int flag = 0;
	if (pl.facing == 3) flag = 1;
	DrawImage(g,
		sprites->img_body[pl.sprite_id + pl.sprite_frame],
		pl.x + g->camera.x,
		pl.y + g->camera.y, flag);
}