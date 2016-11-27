#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "Game.h"
#include "ScaledDraw.h"

// Temp
bool collide(int, int, int, int, int, int, int, int);

void DrawImage(Game* g, ALLEGRO_BITMAP* img, int x, int y, int flags, float opacity){
	if (!g->done) {
		float r_x = (float)g->s_x / (float)g->BWIDTH;
		float r_y = (float)g->s_y / (float)g->BHEIGHT;
		if (img != NULL) {
			int w = al_get_bitmap_width(img);
			int h = al_get_bitmap_height(img);
			if (collide(0, 0, g->BWIDTH, g->BHEIGHT, x, y, w, h)) {
				al_draw_tinted_scaled_bitmap(img,
					al_map_rgba_f(opacity, opacity, opacity, opacity),
					0, 0, w, h, x*r_x, y*r_y, w*r_x, h*r_y, flags);
			}
		} else {
			al_show_native_message_box(NULL, "Error", "File(s) Missing", "Game will quit.", "Quit", 0);
			g->done = true;
		}
	}
}

void DrawScaledImage(Game* g, ALLEGRO_BITMAP* img, int x, int y, int wi, int hi, int flags, float opacity) {
	if (!g->done) {
		float r_x = (float)g->s_x / (float)g->BWIDTH;
		float r_y = (float)g->s_y / (float)g->BHEIGHT;
		if (img != NULL) {
			int w = al_get_bitmap_width(img);
			int h = al_get_bitmap_height(img);
			if (collide(0, 0, g->BWIDTH, g->BHEIGHT, x, y, w, h)) {
				al_draw_tinted_scaled_bitmap(img,
					al_map_rgba_f(opacity, opacity, opacity, opacity),
					0, 0, w, h, x*r_x, y*r_y, (w+wi)*r_x, (h+hi)*r_y, flags);
			}
		} else {
			al_show_native_message_box(NULL, "Error", "File(s) Missing", "Game will quit.", "Quit", 0);
			g->done = true;
		}
	}
}

void DrawRotatedImage(Game* g, ALLEGRO_BITMAP* img, int x, int y, int degree, int flags){
	if (!g->done) {
		float r_x = (float)g->s_x / (float)g->BWIDTH;
		float r_y = (float)g->s_y / (float)g->BHEIGHT;
		int w = al_get_bitmap_width(img);
		int h = al_get_bitmap_height(img);
		if (img != NULL) {
			if (collide(0, 0, g->BWIDTH, g->BHEIGHT, x, y, w, h)) {
				al_draw_scaled_rotated_bitmap(img, w / 2, h / 2, r_x*x, r_y*y, r_x, r_y, (degree * 3.14159 / 180), flags);
			}
		} else {
			al_show_native_message_box(NULL, "Error", "File(s) Missing", "Game will quit.", "Quit", 0);
			g->done = true;
		}
	}
}

void DrawRectangle(Game* g, int x, int y, int w, int h, int re, int gr, int bl, float a){
	float r_x = (float)g->s_x/(float)g->BWIDTH;
	float r_y = (float)g->s_y/(float)g->BHEIGHT;
	if (collide(0, 0, g->BWIDTH, g->BHEIGHT, x, y, w, h)) {
		al_draw_filled_rectangle(r_x*x, r_y*y, r_x*(x + w), r_y*(y + h), al_map_rgba(re, gr, bl, (a*255.0)));
	}
}

void DrawOutline(Game* g, int x, int y, int w, int h, int re, int gr, int bl, float t, int a) {
	float r_x = (float)g->s_x / (float)g->BWIDTH;
	float r_y = (float)g->s_y / (float)g->BHEIGHT;
	if (collide(0, 0, g->BWIDTH, g->BHEIGHT, x, y, w, h)) {
		al_draw_rectangle(r_x*x, r_y*y, r_x*(x + w), r_y*(y + h), al_map_rgba(re, gr, bl, a), t);
	}
}

void DrawCircle(Game* g, int x, int y, float r, int s, int re, int gr, int bl) {
	float r_x = (float)g->s_x / (float)g->BWIDTH;
	float r_y = (float)g->s_y / (float)g->BHEIGHT;
	al_draw_circle(r_x*x, r_y*y, r_x*r, al_map_rgb(re, gr, bl), s);
}