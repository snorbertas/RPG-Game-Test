#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Game.h"
#include "GameHandler.h"
#include "MenuHandler.h"

void HandleMenu(Game* g, ALLEGRO_SAMPLE** sample_sfx){
	HandleIntro(g);
	HandleMenuBackground(g);

	// Check if we're quitting game and we fully faded-out
	if (g->menu.quitting == true && g->fade_out == 1) {
		g->done = true;
	}
}

void HandleIntro(Game* g) {
	if (g->intro_a > 0) g->intro_a -= 0.6;
	if (g->intro_a < 0) g->intro_a = 0;
	if (g->intro_a == 0 && g->Buttons[0].opacity != 1) {
		float opacity = g->Buttons[0].opacity;
		opacity += 0.01;
		if (opacity >= 1) opacity = 1;
		g->Buttons[0].opacity = opacity;
		g->Buttons[241].opacity = opacity;
		g->Buttons[1].opacity = opacity;
		g->Buttons[242].opacity = opacity;
		g->Buttons[2].opacity = opacity;

		g->Buttons[0].y += 1;
		g->Buttons[241].y += 1;
		g->Buttons[1].y += 1;
		g->Buttons[242].y += 1;
		g->Buttons[2].y += 1;
	}
}

void HandleMenuBackground(Game* g) {
	g->menu.bg_x = (-1500 + g->BWIDTH) + (1500 - g->BWIDTH) * (float)((float)(g->BWIDTH - g->pos_x) / (float)g->BWIDTH);
	g->menu.bg_y = (-938 + g->BHEIGHT) + (938 - g->BHEIGHT) * (float)((float)(g->BHEIGHT - g->pos_y) / (float)g->BHEIGHT);
}