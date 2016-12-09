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