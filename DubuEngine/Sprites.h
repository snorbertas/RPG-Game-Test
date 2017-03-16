#pragma once

#define MAX_BODY_SPRITES 28
#define MAX_TILE_SPRITES 55
#define MAX_ICON_SPRITES 4
#define MAX_MISC_SPRITES 5
#define MAX_BACKGROUND_SPRITES 5
#define MAX_INTERFACE_SPRITES 29
#define MAX_BUTTON_SPRITES 38
#define MAX_OBJECT_SPRITES 33
#define MAX_GFX_SPRITES 26

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

enum EObjectSprite {
	/* Map object sprites*/
	EObjectSpriteUndefined = -1, // for objects w/o sprite
	EObjectSpriteBush_SG = 0,
	EObjectSpriteBush_BG = 1,
	EObjectSpriteBush_SD = 2,
	EObjectSpriteBush_BD = 3,
	EObjectSpriteBush_SO = 4,
	EObjectSpriteBush_BO = 5,
	EObjectSpriteTree_BG = 6,
	EObjectSpriteTree_SG = 8,
	EObjectSpriteTree_BD = 10,
	EObjectSpriteTree_SD = 12,
	EObjectSpriteTree_BO = 14,
	EObjectSpriteTree_SO = 16,
	EObjectSpriteFlower_R = 18,
	EObjectSpriteFlower_C = 19,
	EObjectSpriteFlower_P = 20,
	EObjectSpriteGrass_0 = 21,
	EObjectSpriteGrass_1 = 22,
	EObjectSpriteRock_0 = 23,
	EObjectSpriteRock_1 = 24,
	EObjectSpriteRock_2 = 25,
	EObjectSpriteRock_3 = 26,
	EObjectSpriteRock_4 = 27,
	EObjectSpriteRock_5 = 28,
	EObjectSpriteRock_6 = 29,
	EObjectSpriteRock_7 = 30,
	EObjectSpriteRock_8 = 31,
	EObjectSpriteRock_9 = 32,
};