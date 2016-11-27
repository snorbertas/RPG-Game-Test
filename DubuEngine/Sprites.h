#pragma once

#define MAX_BODY_SPRITES 1
#define MAX_TILE_SPRITES 1
#define MAX_ICON_SPRITES 1
#define MAX_MISC_SPRITES 6
#define MAX_BACKGROUND_SPRITES 2
#define MAX_INTERFACE_SPRITES 5
#define MAX_BUTTON_SPRITES 28

#include <allegro5\allegro.h>

struct SpriteStruct {
	/*	Define the types of sprites you want here
		Load them from different folders into separate
		arrays. */
	ALLEGRO_BITMAP *img_body[MAX_BODY_SPRITES];
	ALLEGRO_BITMAP *img_tile[MAX_TILE_SPRITES];
	ALLEGRO_BITMAP *img_icon[MAX_ICON_SPRITES];
	ALLEGRO_BITMAP *img_sprite[MAX_MISC_SPRITES];
	ALLEGRO_BITMAP *img_background[MAX_BACKGROUND_SPRITES];
	ALLEGRO_BITMAP *img_interface[MAX_INTERFACE_SPRITES];
	ALLEGRO_BITMAP *img_button[MAX_BUTTON_SPRITES];
};

