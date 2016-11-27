#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Game.h"
#include "ScaledDraw.h"
#include "MenuRenderer.h"
#include "GameRenderer.h"

void RenderMenu(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font){
	// Temp fix
	ALLEGRO_BITMAP** img_sprite = sprites->img_sprite;
	ALLEGRO_BITMAP** img_background = sprites->img_background;

	// Virtual cursor position
	if (g->menu_x > g->pos_x) {
		g->menu_x -= 5;
		if (g->menu_x < g->pos_x) g->menu_x = g->pos_x;
	} else if (g->menu_x < g->pos_x) {
		g->menu_x += 5;
		if (g->menu_x > g->pos_x) g->menu_x = g->pos_x;
	}
	if (g->menu_y > g->pos_y) {
		g->menu_y -= 5;
		if (g->menu_y < g->pos_y) g->menu_y = g->pos_y;
	} else if (g->menu_y < g->pos_y) {
		g->menu_y += 5;
		if (g->menu_y > g->pos_y) g->menu_y = g->pos_y;
	}

	DrawImage(g, img_background[BACKGROUND_FOREST], -g->menu_x, -g->menu_y, 0);

	// Game Title/Logo
	if (g->intro_a == 0
		&& g->Interfaces[INTERFACE_OPTIONS].visible == false
		&& g->Interfaces[INTERFACE_KEYBOARD].visible == false) {
		DrawImage(g, img_sprite[SPRITE_LOGO], (g->BWIDTH / 2) - 132, 10, 0);
	}

	// Fade
	if (g->intro_a > 0) al_draw_filled_rectangle(0, 0, g->s_x, g->s_y, al_map_rgba(0, 0, 0, g->intro_a));
}

void RenderLoading(Game* g, ALLEGRO_BITMAP** img_background, ALLEGRO_BITMAP** img_sprite, ALLEGRO_FONT** font){
	// r_x, r_y are the x and y ratios for scaling text sizes
	float r_x = (float)g->s_x / (float)g->BWIDTH;
	float r_y = (float)g->s_y / (float)g->BHEIGHT;

	// Loading text
	DrawText(font[0], 255, 255, 255, g->BWIDTH - 100, g->BHEIGHT - 50, ALLEGRO_ALIGN_CENTER, "Loading...");
}