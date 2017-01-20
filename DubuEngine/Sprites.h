#pragma once

#define MAX_BODY_SPRITES 13
#define MAX_TILE_SPRITES 45
#define MAX_ICON_SPRITES 1
#define MAX_MISC_SPRITES 5
#define MAX_BACKGROUND_SPRITES 2
#define MAX_INTERFACE_SPRITES 14
#define MAX_BUTTON_SPRITES 28
#define MAX_OBJECT_SPRITES 23
#define MAX_GFX_SPRITES 4

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
	ALLEGRO_BITMAP *img_object[MAX_OBJECT_SPRITES];
	ALLEGRO_BITMAP *img_gfx[MAX_GFX_SPRITES];
};

enum ObjectSprite {
	/* These are the Map object sprites*/
	Bush_SG = 0,
	Bush_BG = 1,
	Bush_SD = 2,
	Bush_BD = 3,
	Bush_SO = 4,
	Bush_BO = 5,
	Tree_BG = 6,
	Tree_SG = 8,
	Tree_BD = 10,
	Tree_SD = 12,
	Tree_BO = 14,
	Tree_SO = 16,
	Flower_R = 18,
	Flower_C = 19,
	Flower_P = 20,
	Grass_0 = 21,
	Grass_1 = 22
};