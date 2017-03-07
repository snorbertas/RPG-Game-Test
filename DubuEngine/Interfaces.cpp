#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <sstream>
#include <fstream>
#include "Game.h"
#include "GameHandler.h"
#include "ScaledDraw.h"
#include "Interfaces.h"
#include "Radar.h"
#include "MiniMap.h"
#include "Tips.h"
#include "MapObject.h"

inline const char * const BoolToString(bool b)
{
	return b ? "Yes" : "No";
}

void LoadInterfaces(Game* g){
	g->Interfaces = new Interface[MAX_INTERFACES];
	g->Buttons = new Button[MAX_BUTTONS];
	for (int i = 0; i < MAX_INTERFACES; i++) {
		g->Interfaces[i].sprite_id = -1;
		g->Interfaces[i].visible = false;
		g->Interfaces[i].x = g->BWIDTH;
		g->Interfaces[i].y = g->BHEIGHT;
	}
	for (int i = 0; i < MAX_BUTTONS; i++) {
		g->Buttons[i].interface_id = -1;
		g->Buttons[i].sprite_id = 5;
		g->Buttons[i].x = g->BWIDTH;
		g->Buttons[i].y = g->BHEIGHT;
		g->Buttons[i].w = 0;
		g->Buttons[i].h = 0;
	}
	int temp_x, temp_y;

	// Interface 0 (Main Menu)
	g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
	NewInterface(&g->Interfaces[INTERFACE_MAIN_MENU], NO_SPRITE, (g->BWIDTH/2)-(270/2), 210);
	NewButton(&g->Buttons[0], SPRITE_BUTTON_STORY, 0, 0, 270, 50, INTERFACE_MAIN_MENU, 0);			// Singleplayer
	NewButton(&g->Buttons[241], SPRITE_BUTTON_MULTIPLAYER, 0, 55, 270, 50, INTERFACE_MAIN_MENU, 0); // Multiplayer
	NewButton(&g->Buttons[1], SPRITE_BUTTON_OPTIONS, 0, 110, 270, 50, INTERFACE_MAIN_MENU, 0);		// Options
	NewButton(&g->Buttons[242], SPRITE_BUTTON_CREDITS, 0, 165, 270, 50, INTERFACE_MAIN_MENU, 0);	// Credits
	NewButton(&g->Buttons[2], SPRITE_BUTTON_QUIT, 0, 220, 270, 50, INTERFACE_MAIN_MENU, 0);			// Quit
	
	// Interface 1 (Single player mode choice)
	NewInterface(&g->Interfaces[INTERFACE_SINGLE_MODE_CHOICE], NO_SPRITE, (g->BWIDTH / 2), (g->BHEIGHT / 2));
	NewButton(&g->Buttons[15], SPRITE_BUTTON_CANCEL, 188, 236, 112, 45, INTERFACE_SINGLE_MODE_CHOICE);
	temp_x = -320;
	temp_y = -100;
	NewButton(&g->Buttons[3], SPRITE_BUTTON_PLAY, temp_x + 27, temp_y + 250, 142, 35, INTERFACE_SINGLE_MODE_CHOICE);
	temp_x += 220;
	NewButton(&g->Buttons[4], SPRITE_BUTTON_PLAY, temp_x + 27, temp_y + 250, 142, 35, INTERFACE_SINGLE_MODE_CHOICE);
	temp_x += 220;
	NewButton(&g->Buttons[7], SPRITE_BUTTON_PLAY, temp_x + 27, temp_y + 250, 142, 35, INTERFACE_SINGLE_MODE_CHOICE);

	// Interface 2 (Generating map)
	NewInterface(&g->Interfaces[INTERFACE_GENERATING], NO_SPRITE, (g->BWIDTH / 2), (g->BHEIGHT / 2));

	// Interface 3 //Options
	NewInterface(&g->Interfaces[INTERFACE_OPTIONS], SPRITE_INTERFACE_OPTIONS, (g->BWIDTH/2) - 200, (g->BHEIGHT/2) - 200);
	NewButton(&g->Buttons[5], SPRITE_BUTTON_RES, 215, 15, 168, 37, INTERFACE_OPTIONS);
	NewButton(&g->Buttons[6], SPRITE_BUTTON_RES, 215, 71, 168, 37, INTERFACE_OPTIONS);
	NewButton(&g->Buttons[249], SPRITE_BUTTON_CONTROLS, 127, 125, 142, 35, INTERFACE_OPTIONS);

	// Interface 4
	temp_x = g->Interfaces[INTERFACE_OPTIONS].x+g->Buttons[5].x;
	temp_y = g->Interfaces[INTERFACE_OPTIONS].y+g->Buttons[5].y;
	NewInterface(&g->Interfaces[4], NO_SPRITE, temp_x, temp_y);
	NewButton(&g->Buttons[8], SPRITE_BUTTON_RES, 0, 0, 168, 37, 4);
	NewButton(&g->Buttons[9], SPRITE_BUTTON_RES, 0, 37, 168, 37, 4);
	NewButton(&g->Buttons[10], SPRITE_BUTTON_RES, 0, 74, 168, 37, 4);
	NewButton(&g->Buttons[11], SPRITE_BUTTON_RES, 0, 111, 168, 37, 4);
	
	// Interface 5
	temp_x = g->Interfaces[INTERFACE_OPTIONS].x+g->Buttons[6].x;
	temp_y = g->Interfaces[INTERFACE_OPTIONS].y+g->Buttons[6].y;
	NewInterface(&g->Interfaces[5], NO_SPRITE, temp_x, temp_y);
	NewButton(&g->Buttons[12], SPRITE_BUTTON_RES, 0, 0, 168, 37, 5);
	NewButton(&g->Buttons[13], SPRITE_BUTTON_RES, 0, 37, 168, 37, 5);
	NewButton(&g->Buttons[14], SPRITE_BUTTON_RES, 0, 74, 168, 37, 5);
	
	// Interface 6
	g->Interfaces[INTERFACE_VERSION].visible = true;
	NewInterface(&g->Interfaces[INTERFACE_VERSION], NO_SPRITE, 0, 0);

	// Interface 7
	NewInterface(&g->Interfaces[7], 33, g->BWIDTH - 49, g->BHEIGHT - 150);

	// Interface 8 Login interface
	NewInterface(&g->Interfaces[INTERFACE_LOGIN], SPRITE_INTERFACE_LOGIN, (g->BWIDTH / 2) - 190, (g->BHEIGHT / 2) - 150);
	NewButton(&g->Buttons[247], SPRITE_BUTTON_LOGIN, 97, 100, 270, 50, INTERFACE_LOGIN);

	// Interface 9
	NewInterface(&g->Interfaces[INTERFACE_KEYBOARD], SPRITE_INTERFACE_KEYBOARD, (g->BWIDTH / 2) - 200, (g->BHEIGHT / 2) - 200);
	NewButton(&g->Buttons[248], SPRITE_BUTTON_DONE, 127, 256, 142, 35, INTERFACE_KEYBOARD);

	for (int i = 0; i < 8; i++) {
		NewButton(&g->Buttons[250 + i], SPRITE_BUTTON_KEYPICK, 112, 22 + (i * 25), 80, 20, INTERFACE_KEYBOARD);
	}
	for (int i = 0; i < 4; i++) {
		NewButton(&g->Buttons[258 + i], SPRITE_BUTTON_KEYPICK, 294, 22 + (i * 25), 80, 20, INTERFACE_KEYBOARD);
	}
	for (int i = 0; i < 4; i++) {
		NewButton(&g->Buttons[265 + i], SPRITE_BUTTON_KEYPICK, 294, 22 + (4 * 25) + (i * 25), 80, 20, INTERFACE_KEYBOARD);
	}

	// Interface 10
	NewInterface(&g->Interfaces[INTERFACE_PAUSE], NO_SPRITE, 0, 0);
	NewButton(&g->Buttons[262], SPRITE_BUTTON_CONTINUE, (g->BWIDTH / 2) - (270 / 2), 210, 270, 50, INTERFACE_PAUSE);	// Continue
	NewButton(&g->Buttons[263], SPRITE_BUTTON_OPTIONS, (g->BWIDTH / 2) - (270 / 2), 210+60, 270, 50, INTERFACE_PAUSE); // Options
	NewButton(&g->Buttons[264], SPRITE_BUTTON_QUIT, (g->BWIDTH / 2) - (270 / 2), 210+120, 270, 50, INTERFACE_PAUSE);	// Quit

	// Interface 11
	NewInterface(&g->Interfaces[INTERFACE_CREDITS], NO_SPRITE, 0, 0);

	// Interface 12
	NewInterface(&g->Interfaces[INTERFACE_RADAR], NO_SPRITE, g->BWIDTH / 2, g->BHEIGHT- 60);

	// Interface 13
	NewInterface(&g->Interfaces[INTERFACE_STATS], NO_SPRITE, -8, g->BHEIGHT - 94);

	// Interface 14
	NewInterface(&g->Interfaces[INTERFACE_MINI_MAP], NO_SPRITE, g->BWIDTH - 176, g->BHEIGHT - 176);

	// Interface 28 - Welcome Interface
	NewInterface(&g->Interfaces[INTERFACE_WELCOME], NO_SPRITE, 0, 0);
	for (int i = 0; i < 10; i++) {
		/*  Reserving 10 buttons for this interface since it's
			a multipurpose, dynamic interface. */
		NewButton(&g->Buttons[17 + i], SPRITE_BUTTON_PLAY, 0, 0, 142, 35, INTERFACE_WELCOME);
	}

	// Interface 29 - Chat/Console
	NewInterface(&g->Interfaces[INTERFACE_CHAT], NO_SPRITE, 0, g->BHEIGHT - 496);

	// Interface 30 - Scoreboard
	NewInterface(&g->Interfaces[INTERFACE_SCORE], NO_SPRITE, (g->BWIDTH / 2) - 210, (g->BHEIGHT / 2) - 300);

	// Interface 99 (on-top)
	temp_x = g->BWIDTH / 2 - 250;
	temp_y = g->BHEIGHT / 2 - 84;
	NewInterface(&g->Interfaces[INTERFACE_MESSAGE], SPRITE_INTERFACE_MESSAGE, temp_x, temp_y);
	NewButton(&g->Buttons[16], SPRITE_BUTTON_OKAY, 195, 110, 112, 43, INTERFACE_MESSAGE);
	g->Message1 = "Hello, this is the first line of the message";
	g->Message2 = "and this is the second line of memes ex dee";
}

void NewInterface(Interface* in, int sprite, int x, int y){
	in->sprite_id = sprite;
	in->x = x;
	in->y = y;
	
}

void NewButton(Button* b, int sprite, int x, int y, int w, int h, int iid, float opacity){
	b->sprite_id = sprite;
	b->x = x;
	b->y = y;
	b->w = w;
	b->h = h;
	b->interface_id = iid; // This is the interface id that the button belongs to
	b->opacity = opacity;
}

void RenderInterfaces(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font){
	// These shortened pointers make my life easier when I read it
	ALLEGRO_BITMAP** img_sprite = sprites->img_sprite;
	ALLEGRO_BITMAP** img_background = sprites->img_background;
	ALLEGRO_BITMAP** img_interface = sprites->img_interface;
	ALLEGRO_BITMAP** img_button = sprites->img_button;

	// Render Interfaces
	for(int i = 0; i < MAX_INTERFACES; i++){
		if(g->Interfaces[i].visible == true){
			if (i == 99) {
				DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0, 0.75);
			}

			// Darken for pause menu
			if (i == INTERFACE_PAUSE ||
				(g->scene == 2 && (i == INTERFACE_OPTIONS || i == INTERFACE_KEYBOARD))) {
				DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0, 0.95);
			}

			// Draw Interface image
			DrawImage(g, img_interface[g->Interfaces[i].sprite_id],
				g->Interfaces[i].x,
				g->Interfaces[i].y,
				0, g->Interfaces[i].opacity);

			// Draw interface boxes before buttons
			if (i == INTERFACE_SINGLE_MODE_CHOICE) {
				// Initial x/y
				int x = g->Interfaces[i].x - 320;
				int y = g->Interfaces[i].y - 100;

				// Menu
				DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y + 320, 640, 80, 0.8);

				// Bone Hunt
				DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, 200, 300, 0.8);

				// Bonesweeper
				x += 220;
				DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, 200, 300, 0.8);

				// Surival
				x += 220;
				DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, 200, 300, 0.8);
			}

			if (i == INTERFACE_WELCOME) {
				// Darken background
				DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0, 0.5);

				// Initial x/y
				int x = g->welcome_interface.X();
				int y = g->welcome_interface.Y();
				int w = g->welcome_interface.Width();
				int h = g->welcome_interface.Height();
				DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, w, h, 0.9);
			}

			// Loop trough all buttons
			for(int j = 0; j < MAX_BUTTONS; j++){
				if(g->Buttons[j].interface_id == i){
					if (g->Buttons[j].visible) {
						if (g->pos_x > g->Buttons[j].x + g->Interfaces[i].x &&
							g->pos_x < g->Buttons[j].x + g->Buttons[j].w + g->Interfaces[i].x &&
							g->pos_y > g->Buttons[j].y + g->Interfaces[i].y &&
							g->pos_y < g->Buttons[j].y + g->Buttons[j].h + g->Interfaces[i].y &&
							InterfaceIsOnTop(g, i)) {
							DrawImage(g, img_button[g->Buttons[j].sprite_id + 1],
								g->Buttons[j].x + g->Interfaces[i].x,
								g->Buttons[j].y + g->Interfaces[i].y, 0, g->Buttons[j].opacity);
						} else {
							DrawImage(g, img_button[g->Buttons[j].sprite_id],
								g->Buttons[j].x + g->Interfaces[i].x,
								g->Buttons[j].y + g->Interfaces[i].y, 0, g->Buttons[j].opacity);
						}
					}
				}
			}
			
			float r_x = (float)g->s_x/(float)g->BWIDTH;
			float r_y = (float)g->s_y/(float)g->BHEIGHT;
			switch(i){
				case INTERFACE_SINGLE_MODE_CHOICE:
				{
					// Initial x/y
					int x = g->Interfaces[i].x - 320;
					int y = g->Interfaces[i].y - 100;

					// Menu
					DrawText(font[9], 255, 255, 255, x + 10, y + 330, ALLEGRO_ALIGN_LEFT,
						"TODO: Add <Customize> <Shop> <Achievements> buttons here");

					// Bone Hunt
					DrawText(font[6], 10, 255, 32, x + 100, y + 5, ALLEGRO_ALIGN_CENTER, "Bone Hunt");
					DrawText(font[9], 255, 255, 255, x + 100, y + 180, ALLEGRO_ALIGN_CENTER, "Claim all the territory and");
					DrawText(font[9], 255, 255, 255, x + 100, y + 195, ALLEGRO_ALIGN_CENTER, "find all the bones before it");
					DrawText(font[9], 255, 255, 255, x + 100, y + 210, ALLEGRO_ALIGN_CENTER, "gets dark. Wolves come out");
					DrawText(font[9], 255, 255, 255, x + 100, y + 225, ALLEGRO_ALIGN_CENTER, "when it's night time.");
					
					// Thumbnail
					for (int yi = 0; yi < 2; yi++) {
						for (int xi = 0; xi < 2; xi++) {
							DrawImage(g, sprites->img_tile[4],
								x + (xi * Map::TILE_SIZE) + 36,
								y + (yi * Map::TILE_SIZE) + 45, 0);
						}
					}
					DrawImage(g, sprites->img_object[13], x + 45, y + 26, 0);
					DrawImage(g, sprites->img_object[12], x + 45, y + 90, 0);
					DrawImage(g, sprites->img_gfx[4], x + 85, y + 92, 0);
					DrawImage(g, sprites->img_body[12], x + 85, y + 92, 0);
					DrawScaledImage(g, sprites->img_interface[5], x + 102, y + 132, -20, -20, 0);

					// Bonesweeper
					x += 220;
					DrawText(font[0], 255, 188, 10, x + 100, y + 10, ALLEGRO_ALIGN_CENTER, "BoneSweeper");
					DrawText(font[9], 255, 255, 255, x + 100, y + 180, ALLEGRO_ALIGN_CENTER, "Find all the bones, but be");
					DrawText(font[9], 255, 255, 255, x + 100, y + 195, ALLEGRO_ALIGN_CENTER, "careful. The island is full");
					DrawText(font[9], 255, 255, 255, x + 100, y + 210, ALLEGRO_ALIGN_CENTER, "of bombs! Dig with caution");
					DrawText(font[9], 255, 255, 255, x + 100, y + 225, ALLEGRO_ALIGN_CENTER, "by marking the bombs.");

					// Thumbnail
					for (int yi = 0; yi < 2; yi++) {
						for (int xi = 0; xi < 2; xi++) {
							DrawImage(g, sprites->img_tile[19],
								x + (xi * Map::TILE_SIZE) + 36,
								y + (yi * Map::TILE_SIZE) + 45, 0);
						}
					}
					DrawImage(g, sprites->img_object[5], x + 30, y + 26, 0);
					DrawImage(g, sprites->img_object[4], x + 105, y + 95, 0);
					DrawImage(g, sprites->img_gfx[3], x + 70, y + 85, 0);
					DrawImage(g, sprites->img_body[14], x + 70, y + 85, 0);
					DrawImage(g, sprites->img_gfx[2], x + 70, y + 90, ALLEGRO_FLIP_VERTICAL, 0.4);


					//DrawRectangle(g, x + 8, y + 8, 200 - 16, 300 - 16, 0, 0, 0, 0.9);
					//DrawText(font[0], 128, 0, 0, x + 100, y + 130, ALLEGRO_ALIGN_CENTER, "Coming Soon!");
					//g->Buttons[4].visible = false;

					// Surival
					x += 220;
					DrawText(font[6], 10, 143, 255, x + 100, y + 5, ALLEGRO_ALIGN_CENTER, "Survival");
					DrawText(font[9], 255, 255, 255, x + 100, y + 180, ALLEGRO_ALIGN_CENTER, "Interact with other canines");
					DrawText(font[9], 255, 255, 255, x + 100, y + 195, ALLEGRO_ALIGN_CENTER, "to survive in a dangerious");
					DrawText(font[9], 255, 255, 255, x + 100, y + 210, ALLEGRO_ALIGN_CENTER, "island. Claim all territory");
					DrawText(font[9], 255, 255, 255, x + 100, y + 225, ALLEGRO_ALIGN_CENTER, "to become the alpha dog!");
					//DrawRectangle(g, x + 8, y + 8, 200 - 16, 300 - 16, 0, 0, 0, 0.9);
					//DrawText(font[0], 128, 0, 0, x + 100, y + 130, ALLEGRO_ALIGN_CENTER, "Coming Soon!");
					//g->Buttons[7].visible = false;
					
					// Thumbnail
					for (int xi = 0; xi < 2; xi++) {
						DrawImage(g, sprites->img_tile[32],
							x + (xi * Map::TILE_SIZE) + 36,
							y + (0 * Map::TILE_SIZE) + 45, 0);
					}
					for (int xi = 0; xi < 2; xi++) {
						DrawImage(g, sprites->img_tile[35],
							x + (xi * Map::TILE_SIZE) + 36,
							y + (1 * Map::TILE_SIZE) + 45, 0);
					}
					DrawImage(g, sprites->img_body[3], x + 70, y + 108, 0);


					break;
				}
				case INTERFACE_GENERATING:
					if (!g->menu.done_loading) {
						// Initial x/y
						int x = g->Interfaces[i].x - 150;
						int y = g->Interfaces[i].y - 50;
						DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, 300, 60, 0.8);

						// Calculate dots
						x += 60;
						if (g->menu.loading_timer > SecondsToTicks(0.75)) {
							DrawText(font[0], 255, 255, 255, x, y + 15, ALLEGRO_ALIGN_LEFT, "Generating Map");
						}
						else if (g->menu.loading_timer > SecondsToTicks(0.5)) {
							DrawText(font[0], 255, 255, 255, x, y + 15, ALLEGRO_ALIGN_LEFT, "Generating Map.");
						}
						else if (g->menu.loading_timer > SecondsToTicks(0.25)) {
							DrawText(font[0], 255, 255, 255, x, y + 15, ALLEGRO_ALIGN_LEFT, "Generating Map..");
						}
						else {
							DrawText(font[0], 255, 255, 255, x, y + 15, ALLEGRO_ALIGN_LEFT, "Generating Map...");
						}

						// Tips
						DrawText(font[0], 255, 255, 255, g->Interfaces[i].x, g->BHEIGHT - 80, ALLEGRO_ALIGN_CENTER, GetTip(g->menu.tip_id));
					}
					break;
				case 3:
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 222, g->Interfaces[i].y + 22, ALLEGRO_ALIGN_LEFT, "%i x %i", g->s_x, g->s_y);
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 222, g->Interfaces[i].y + 78, ALLEGRO_ALIGN_LEFT, GetWindowMode(g->window_mode));
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 22, g->Interfaces[i].y + 20, ALLEGRO_ALIGN_LEFT, "Resolution:");
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 22, g->Interfaces[i].y + 75, ALLEGRO_ALIGN_LEFT, "Window Size:");
					break;
				case 4:	
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 7, ALLEGRO_ALIGN_LEFT, "1024 x 576");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 44, ALLEGRO_ALIGN_LEFT, "1280 x 720");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 81, ALLEGRO_ALIGN_LEFT, "1600 x 900");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 118, ALLEGRO_ALIGN_LEFT, "1920 x 1080");
					break;
				case 5:
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 7, ALLEGRO_ALIGN_LEFT, "Window");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 44, ALLEGRO_ALIGN_LEFT, "Fullscreen");
					DrawText(font[0], 255, 255, 255, g->Interfaces[i].x + 7, g->Interfaces[i].y + 81, ALLEGRO_ALIGN_LEFT, "Borderless");
					break;
				case INTERFACE_VERSION: {
					DrawText(font[2], 255, 255, 255, 0, 0, ALLEGRO_ALIGN_LEFT, ENGINE_VERSION);
					int tick_diff = (int)((g->new_time - g->old_time) * 1000);
					int r = 0;
					int gr = 255;
					int b = 0;
					if (tick_diff > (int)(1000 / TPS)) {
						r = 255;
						gr = 0;
					} else if (tick_diff < (int)(1000 / TPS)) {
						gr = 0;
						b = 255;
					}
					DrawText(font[2], r, gr, b, 0, 15, ALLEGRO_ALIGN_LEFT, "Tick interval: %i/%ims", tick_diff, (1000/TPS));
					if (g->connected) {
						DrawText(font[2], 255, 0, 255, 0, 30, ALLEGRO_ALIGN_LEFT, "Ping: %i", g->ping);
					}
					DrawText(font[2], 255, 255, 0, 0, 45, ALLEGRO_ALIGN_LEFT, "Map Seed: %i", g->map.seed);
					DrawText(font[2], 255, 255, 10, 0, 60, ALLEGRO_ALIGN_LEFT, "Render Mode: %i", g->map.GetRenderMode());
					DrawText(font[2], 255, 255, 10, 0, 75, ALLEGRO_ALIGN_LEFT, "Ticks To Move: %i", g->pl.ticks_to_move);
					DrawText(font[2], 255, 255, 10, 0, 90, ALLEGRO_ALIGN_LEFT, "Velocity: %i", g->pl.velocity);
					DrawText(font[2], 255, 255, 10, 0, 105, ALLEGRO_ALIGN_LEFT, "Ticks To Anim: %i", g->pl.ticks_to_anim);
					if (g->debug.showMegaMap) {
						DrawMiniMap(g, sprites, (g->BWIDTH - 500) / 2, (g->BHEIGHT - 500 ) / 2, 500, 500);
					}
					break;
				}
				case 7:
					g->music_volume_cd--;
					if (g->music_volume_cd <= 0) g->Interfaces[i].visible = false;
					DrawRectangle(g, g->Interfaces[i].x+3, g->Interfaces[i].y+146, 43, 0-g->music_volume*100, 132, 187, 234);
					break;
				case INTERFACE_LOGIN:
				{
					DrawText(font[1], 0, 0, 0, g->Interfaces[i].x + 110, g->Interfaces[i].y + 22, ALLEGRO_ALIGN_LEFT, g->logini.username_input.c_str());
					DrawText(font[1], 0, 0, 0, g->Interfaces[i].x + 110, g->Interfaces[i].y + 64, ALLEGRO_ALIGN_LEFT, g->logini.password_input_mask.c_str());
					int input_lenght = 0;
					if (g->logini.type_username) {
						input_lenght = al_get_text_width(font[5], g->logini.username_input.c_str());
					} else {
						input_lenght = al_get_text_width(font[5], g->logini.password_input_mask.c_str());
					}
					if (g->logini.type_indicator_count < 60 && !g->logini.logging_in) {
						if (g->logini.type_username) {
							DrawRectangle(g, g->Interfaces[i].x + 110 + input_lenght, g->Interfaces[i].y + 22, 2, 16, 255, 255, 255);
						} else {
							DrawRectangle(g, g->Interfaces[i].x + 110 + input_lenght, g->Interfaces[i].y + 64, 2, 16, 255, 255, 255);
						}
					}
					if (g->logini.logging_in) {
						DrawText(font[0], 10, 10, 205,
							g->Interfaces[INTERFACE_LOGIN].x + 185,
							g->Interfaces[INTERFACE_LOGIN].y + 110,
							ALLEGRO_ALIGN_CENTER, "Connecting to server...");
						DrawRotatedImage(g, img_sprite[SPRITE_INTERFACE_CONNECTING],
							g->Interfaces[INTERFACE_LOGIN].x + 185,
							g->Interfaces[INTERFACE_LOGIN].y + 185,
							g->logini.loading_angle, 0);
						g->logini.loading_angle+=3;
						if (g->logini.loading_angle > 360) g->logini.loading_angle = 0;
					}
					break;
				}
				case INTERFACE_KEYBOARD:
				{
					int offset = 0;
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21,
						ALLEGRO_ALIGN_LEFT, "Move Left:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.left_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Move Right:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.right_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Move Up:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.up_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Move Down:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.down_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Camera:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.camera_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Dig:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.dig_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Sniff:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.sniff_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Chat:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.chat_bind));
					break;
				}
				case INTERFACE_PAUSE:
				{
					//
					break;
				}
				case INTERFACE_CREDITS:
				{
					//
					DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0, 0.5);
					int credits_x = g->BWIDTH / 2;
					int credits_y = g->BHEIGHT / 2;
					DrawImage(g, img_button[SPRITE_BUTTON_CREDITS], 15, 250, 0);
					credits_x = 100;
					credits_y -= 80;
					DrawRectangle(g, 95, 300, 560, 290, 0, 0, 0, 0.1);
					DrawText(font[0], 255, 200, 200, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "[Programming]");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Norbert S.");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "https://github.com/snorbertas");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Alexander K.");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "https://github.com/Aleksander92");
					//credits_x += 300;
					DrawText(font[0], 255, 200, 200, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "[Graphic Art]");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Kenney");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "http://kenney.nl/");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Ryutard (Roo)");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "https://www.instagram.com/ryutard/");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Norbert S.");
					credits_x += 330;
					credits_y = g->BHEIGHT / 2 - 80;
					DrawText(font[0], 255, 200, 200, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "[Libraries]");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Allegro");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "http://liballeg.org/");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Bass");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "http://www.un4seen.com/");
					credits_x += 330;
					credits_y = g->BHEIGHT / 2;
					DrawRectangle(g, 755, 355, 470, 185, 0, 0, 0, 0.2);
					DrawText(font[2], 255, 255, 255, credits_x, credits_y, ALLEGRO_ALIGN_LEFT,
						"This is a simple 2D game engine with socket support.");
					DrawText(font[2], 255, 255, 255, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT,
						"Whenever I have an idea for a multiplayer game for windows,");
					DrawText(font[2], 255, 255, 255, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT,
						"I program it on top of this engine.");
					DrawText(font[2], 255, 255, 255, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT,
						"All of this is for practice and educational purposes.");
					DrawText(font[2], 255, 255, 255, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT,
						"You may use the engine to build your own games, but please");
					DrawText(font[2], 255, 255, 255, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT,
						"give credit and link to my github.");
					DrawText(font[0], 170, 170, 170, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT,
						"Press [ESC] to close...");
					break;
				}
				case INTERFACE_RADAR: {
					RenderRadar(g, font, sprites);
					if (g->pl.dig_timer > 0) {
						// Prog bar
						int x = g->BWIDTH / 2 - 100;
						int y = g->BHEIGHT / 2 - 50;
						int length_empty = 200;
						int length_red = length_empty - ((float)g->pl.dig_timer / (float)g->pl.dig_duration) * length_empty;
						DrawBar(g, sprites, BarType::EMPTY, x, y, length_empty);
						DrawBar(g, sprites, BarType::RED, x, y, length_red);

						// Text
						DrawText(font[3], 232, 106, 23, (g->BWIDTH / 2), y - 25, ALLEGRO_ALIGN_CENTER, "Digging...");
					}
					if (g->pl.pee_timer > 0) {
						// Prog bar
						int x = g->BWIDTH / 2 - 100;
						int y = g->BHEIGHT / 2 - 50;
						int length_empty = 200;
						int length_yellow = length_empty - ((float)g->pl.pee_timer / (float)g->pl.pee_duration) * length_empty;
						DrawBar(g, sprites, BarType::EMPTY, x, y, length_empty);
						DrawBar(g, sprites, BarType::YELLOW, x, y, length_yellow);

						// Text
						DrawText(font[3], 232, 210, 23, (g->BWIDTH / 2), y - 25, ALLEGRO_ALIGN_CENTER, "Peeing...");
					}
					if (g->pl.drink_timer > 0) {
						// Prog bar
						int x = g->BWIDTH / 2 - 100;
						int y = g->BHEIGHT / 2 - 50;
						int length_empty = 200;
						int length_blue = length_empty - ((float)g->pl.drink_timer / (float)g->pl.drink_duration) * length_empty;
						DrawBar(g, sprites, BarType::EMPTY, x, y, length_empty);
						DrawBar(g, sprites, BarType::BLUE, x, y, length_blue);

						// Text
						DrawText(font[3], 23, 149, 232, (g->BWIDTH / 2), y - 25, ALLEGRO_ALIGN_CENTER, "Drinking...");
					}
					break;
				}
				case INTERFACE_STATS:
				{
					int x = g->Interfaces[i].x;
					int y = g->Interfaces[i].y;
					DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x, y, 306, 106);

					// Player avatar
					// TODO: Perhaps make it customizable? etc.
					x += 2;
					DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x + 10, y + 10, 80, 80, 0.8);
					DrawImage(g, sprites->img_body[0], x + 18, y + 16, 0);

					// Player name
					x += 95;
					DrawText(font[0], 64, 32, 64, x, y, ALLEGRO_ALIGN_LEFT, g->pl.name.c_str());

					// Bones found
					y += 6;
					DrawText(font[2], 255, 255, 255, x, y + 22, ALLEGRO_ALIGN_LEFT, "Bones Found: %i", g->pl.bones_found);

					// Bladder
					float bladder_fraction = ((float)g->pl.pee_ammo / (float)g->pl.pee_max);
					int bladder_lenght = bladder_fraction * 200;
					DrawBar(g, sprites, BarType::EMPTY, x, y + 44, 200);
					if(bladder_lenght > 0) DrawBar(g, sprites, BarType::BLUE, x, y + 44, bladder_lenght);
					DrawText(font[2], 64, 64, 255, x + 100, y + 42, ALLEGRO_ALIGN_CENTER, "Bladder");

					// Stamina
					float stamina_fraction = ((float)g->pl.stamina_left / (float)g->pl.stamina_max);
					int stamina_lenght = stamina_fraction * 200;
					DrawBar(g, sprites, BarType::EMPTY, x, y + 66, 200);
					DrawBar(g, sprites, BarType::RED, x, y + 66, stamina_lenght);
					DrawText(font[2], 170, 64, 64, x + 100, y + 64, ALLEGRO_ALIGN_CENTER, "Stamina");

					// Timer
					x = g->BWIDTH / 2;
					y = -8;
					int gb = 255;	// Green-Blue
					string format = "%i:%i";

					// Add extra 0 infront of seconds so we maintain the M:SS format
					if (g->game_duration.seconds < 10) {
						format = "%i:0%i";
					}

					// Turn last 10 seconds red
					if (g->game_duration.inverted) {
						if (g->game_duration.minutes == 0 && g->game_duration.seconds < 10) {
							gb = 64;
						}
					}

					// Draw frame
					DrawInterfaceBox(g, sprites, InterfaceBoxType::BROWN, x - 50, y, 100, 46, 0.8);

					// Write the timer
					DrawText(font[6], 255, gb, gb, x, y, ALLEGRO_ALIGN_CENTER,
						format.c_str(), g->game_duration.minutes, g->game_duration.seconds);
					break;
				}
				case INTERFACE_MINI_MAP:
				{
					DrawMiniMap(g, sprites, g->Interfaces[i].x, g->Interfaces[i].y);
					break;
				}
				case INTERFACE_WELCOME:
					g->welcome_interface.InitSpecialElements(g);
					g->welcome_interface.DrawAllElements(g, sprites, font);
					break;
				case INTERFACE_CHAT:
					if (g->chat.show_chat && g->scene == 1) {
						DrawRectangle(g, g->Interfaces[i].x + 2, g->Interfaces[i].y + 2, 500 - 4, 400 - 4, 0, 0, 0, 0.3);
						DrawRectangle(g, g->Interfaces[i].x, g->Interfaces[i].y, 500, 400, 0, 0, 0, 0.1);
						DrawRectangle(g, g->Interfaces[i].x + 2, g->Interfaces[i].y + 378, 500 - 4, 20, 0, 0, 0, 0.3);
						DrawText(font[1], 255, 255, 255,
							g->Interfaces[i].x + 2,
							g->Interfaces[i].y + 376,
							ALLEGRO_ALIGN_LEFT, "%s: %s", g->pl.name.c_str(), g->chat.msg_input.c_str());
						int input_lenght = al_get_text_width(font[5], g->pl.name.c_str())
										 + al_get_text_width(font[5], ": ")
										 + al_get_text_width(font[5], g->chat.msg_input.c_str());
						if (g->chat.type_chat && g->chat.chat_indicator_count < 60) {
							DrawRectangle(g, g->Interfaces[i].x + 2 + input_lenght, g->Interfaces[i].y + 380, 2, 16, 255, 255, 255);
						}

						// Chat messages
						for (int j = 0; j < MAX_CHAT_MESSAGES; j++) {
							DrawText(font[1], g->chat.messages[j].r, g->chat.messages[j].g, g->chat.messages[j].b,
								g->Interfaces[i].x + 2, g->Interfaces[i].y + 358 - (j * 18), ALLEGRO_ALIGN_LEFT,
								g->chat.messages[j].parent.c_str());
							int msg_lenght = al_get_text_width(font[5], g->chat.messages[j].parent.c_str());
							DrawText(font[1], 255, 255, 255, g->Interfaces[i].x + 2 + msg_lenght, g->Interfaces[i].y + 358 - (j * 18),
								ALLEGRO_ALIGN_LEFT,	g->chat.messages[j].message.c_str());
						}
					} else if (g->chat.ghost > 0) {
						// Chat messages
						for (int j = 0; j < MAX_CHAT_MESSAGES; j++) {
							DrawText(font[1], g->chat.messages[j].r, g->chat.messages[j].g, g->chat.messages[j].b,
								g->Interfaces[i].x + 2, g->Interfaces[i].y + 358 - (j * 18), ALLEGRO_ALIGN_LEFT,
								g->chat.messages[j].parent.c_str());
							int msg_lenght = al_get_text_width(font[5], g->chat.messages[j].parent.c_str());
							DrawText(font[1], 255, 255, 255, g->Interfaces[i].x + 2 + msg_lenght, g->Interfaces[i].y + 358 - (j * 18),
								ALLEGRO_ALIGN_LEFT, g->chat.messages[j].message.c_str());
						}
					}
					break;
				case INTERFACE_SCORE:
				{
					if (g->score.show_score) {
						int x = g->Interfaces[i].x;
						int y = g->Interfaces[i].y;
						int width = 420;
						int height = 525;

						// Draw frame
						DrawRectangle(g, x, y, width, height, 0, 0, 0, 0.3);
						DrawRectangle(g, x, y, width, 25, 64, 0, 64, 0.3);
						DrawOutline(g, x, y, width, height, 0, 0, 0, 1);

						// Draw columns
						DrawOutline(g, x, y, 155, height, 0, 0, 0, 1, 128);
						DrawOutline(g, x, y, 205, height, 0, 0, 0, 1, 128);
						DrawOutline(g, x, y, 270, height, 0, 0, 0, 1, 128);
						DrawOutline(g, x, y, 360, height, 0, 0, 0, 1, 128);

						// Draw rows
						for (int iy = 0; iy < g->MAX_PLAYERS + 2; iy++) {
							DrawOutline(g, x, y, width, 25 * iy, 0, 0, 0, 1, 128);
						}

						// Draw key
						x += 5;
						DrawText(font[1], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "Player");
						x += 155;
						DrawText(font[1], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "Ping");
						x += 50;
						DrawText(font[1], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "Bones");
						x += 65;
						DrawText(font[1], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "Territory");
						x += 90;
						DrawText(font[1], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "Score");

						// Error message if not in multiplayer
						if (!g->connected) {
							x = g->Interfaces[i].x + (width / 2);
							y += (height / 2) - 4;
							DrawRectangle(g, x - 180, y - 3, 360, 40, 5, 0, 0, 0.8);
							DrawOutline(g, x - 180, y - 3, 360, 40, 0, 0, 0, 1);
							DrawText(font[0], 255, 64, 64, x, y, ALLEGRO_ALIGN_CENTER, "Not Connected To Multiplayer");
							break;
						}

						// Draw scores
						// Set starting point
						y += 28;
						x = g->Interfaces[i].x + 4;

						// Loop trough all score
						for (int sid = 0; sid < g->MAX_PLAYERS; sid++) {
							ScoreInfo* sinfo = &g->score.score_info[sid];
							if (sinfo->active) {
								// Draw Icon
								DrawImage(g, sprites->img_icon[sinfo->icon_id], x, y, 0);

								// Write name
								x += 20;
								DrawText(font[2], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, sinfo->name);

								// Write ping
								x += 135;
								DrawText(font[2], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "%i", sinfo->ping);

								// Write bones
								x += 50;
								DrawText(font[2], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "%i", sinfo->bones);

								// Write territory
								x += 65;
								DrawText(font[2], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "%i", sinfo->territory);

								// Write score
								x += 90;
								DrawText(font[2], 255, 255, 255, x, y, ALLEGRO_ALIGN_LEFT, "%i", sinfo->score);

								// Reset x and Vertical bump for next row
								y += 25;
								x = g->Interfaces[i].x + 4;
							}
						}
					}
				}
					break;
				case 99:
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 250, g->Interfaces[i].y + 20, ALLEGRO_ALIGN_CENTER, g->Message1);
					DrawText(font[0], 0, 0, 0, g->Interfaces[i].x + 250, g->Interfaces[i].y + 52, ALLEGRO_ALIGN_CENTER, g->Message2);
					break;
			}
		}
	}

	// Fade
	if (g->fade_in > SecondsToTicks(0.1)) {
		float a = ((float)(g->fade_in - SecondsToTicks(0.1)) / (float)SecondsToTicks(2.0));
		DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0, a);
	}
	if (g->fade_out > SecondsToTicks(0.1)) {
		float a = ((float)(g->fade_out - SecondsToTicks(0.1)) / (float)SecondsToTicks(2.0));
		DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0, 1-a);
	} else if (g->fade_out > 0) {
		DrawRectangle(g, 0, 0, g->BWIDTH, g->BHEIGHT, 0, 0, 0);
	}

	// Cursor
	DrawImage(g, img_sprite[SPRITE_CURSOR], g->pos_x, g->pos_y, 0);
}

const char* GetWindowMode(int wid){
	if(wid == ALLEGRO_WINDOWED) return "Window";
	if(wid == ALLEGRO_FULLSCREEN) return "Fullscreen";
	if(wid == ALLEGRO_FULLSCREEN_WINDOW) return "Borderless";
	return "???";
}

bool InterfaceIsOnTop(Game* g, int id) {
	int top_i = -1;
	for (int i = 0; i < MAX_INTERFACES; i++) {
		if (g->Interfaces[i].visible == true) {
			// TODO
			if (i != 6 && i != 2 && i != 7 && i != 12 &&
				i != 31 && i != 32 && i != 33 && i != 29 &&
				i != 27 && i != 34 && i != 35 && i != 26 &&
				i != 30 && i != 13 && i != 14) {
				top_i = i;
			}
		}
	}
	if (top_i == id) {
		return true;
	} else {
		return false;
	}
}

void HideAllInterfaces(Game* g, int except) {
	for (int i = 0; i < MAX_INTERFACES; i++) {
		if(i != except) g->Interfaces[i].visible = false;
	}
}

void HandleInterfaces(Game* g) {
	g->fade_in--;
	if (g->fade_in < 0) g->fade_in = 0;
	g->fade_out--;
	if (g->fade_out < 0) g->fade_out = 0;
	
	HandleChatInterface(g);
	HandleLoginInterface(g);
	if(g->Interfaces[INTERFACE_WELCOME].visible == true) g->welcome_interface.Handle();
}

void HandleChatInterface(Game* g) {
	g->chat.chat_indicator_count++;
	if (g->chat.chat_indicator_count >= 120) g->chat.chat_indicator_count = 0;
	if (g->chat.backspace_timer > 0) g->chat.backspace_timer--;
	if (g->chat.ghost > 0) g->chat.ghost--;
	if (g->keys.backspace) {
		if (g->chat.type_chat && g->chat.show_chat && g->chat.backspace_timer <= 0) {
			g->chat.backspace_timer_wait--;
			if (g->chat.backspace_timer_wait <= 0) {
				if (g->chat.msg_input != "") {
					g->chat.msg_input.pop_back();
					g->chat.backspace_timer = 5;
				}
			}
		}
	}
}

void HandleLoginInterface(Game* g) {
	if (!g->logini.logging_in) {
		g->logini.type_indicator_count++;
		if (g->logini.type_indicator_count >= 120) g->logini.type_indicator_count = 0;
		if (g->logini.backspace_timer > 0) g->logini.backspace_timer--;
		if (g->keys.backspace) {
			if (g->Interfaces[INTERFACE_LOGIN].visible && g->logini.backspace_timer <= 0) {
				g->logini.backspace_timer_wait--;
				if (g->logini.backspace_timer_wait <= 0) {
					if (g->logini.type_username == true) {
						if (g->logini.username_input != "") {
							g->logini.username_input.pop_back();
							g->logini.backspace_timer = 5;
						}
					}
					else {
						if (g->logini.password_input != "") {
							g->logini.password_input.pop_back();
							g->logini.password_input_mask.pop_back();
							g->logini.backspace_timer = 5;
						}
					}
				}
			}
		}
	}
}

const char* GetKeyName(int key_id) {
	if (key_id == -1) return "(none)";
	if (key_id == ALLEGRO_KEY_Q) return "Q";
	if (key_id == ALLEGRO_KEY_W) return "W";
	if (key_id == ALLEGRO_KEY_E) return "E";
	if (key_id == ALLEGRO_KEY_R) return "R";
	if (key_id == ALLEGRO_KEY_T) return "T";
	if (key_id == ALLEGRO_KEY_Y) return "Y";
	if (key_id == ALLEGRO_KEY_U) return "U";
	if (key_id == ALLEGRO_KEY_I) return "I";
	if (key_id == ALLEGRO_KEY_O) return "O";
	if (key_id == ALLEGRO_KEY_P) return "P";
	if (key_id == ALLEGRO_KEY_A) return "A";
	if (key_id == ALLEGRO_KEY_S) return "S";
	if (key_id == ALLEGRO_KEY_D) return "D";
	if (key_id == ALLEGRO_KEY_F) return "F";
	if (key_id == ALLEGRO_KEY_G) return "G";
	if (key_id == ALLEGRO_KEY_H) return "H";
	if (key_id == ALLEGRO_KEY_J) return "J";
	if (key_id == ALLEGRO_KEY_K) return "K";
	if (key_id == ALLEGRO_KEY_L) return "L";
	if (key_id == ALLEGRO_KEY_Z) return "Z";
	if (key_id == ALLEGRO_KEY_X) return "X";
	if (key_id == ALLEGRO_KEY_C) return "C";
	if (key_id == ALLEGRO_KEY_V) return "V";
	if (key_id == ALLEGRO_KEY_B) return "B";
	if (key_id == ALLEGRO_KEY_N) return "N";
	if (key_id == ALLEGRO_KEY_M) return "M";
	if (key_id == ALLEGRO_KEY_LEFT) return "Left";
	if (key_id == ALLEGRO_KEY_RIGHT) return "Right";
	if (key_id == ALLEGRO_KEY_UP) return "Up";
	if (key_id == ALLEGRO_KEY_DOWN) return "Down";
	if (key_id == ALLEGRO_KEY_SPACE) return "Space";
	if (key_id == ALLEGRO_KEY_ENTER) return "Enter";
	return "???";
}

void SubmitMessage(Game* g, const char* msg) {
	if (msg[0] == '/') {
		HandleCommand(g, msg);
	} else {
		if (g->connected) {
			PacketBuffer2* message = new PacketBuffer2(PACKET_TYPE_CHAT_MESSAGE);
			AddToBuffer(message->buffer_1, g->pl.name.c_str());
			AddToBuffer(message->buffer_2, msg);
			AddPacketToQueue(g, message);
		} else {
			AddChatMessage(g->chat, g->pl.name.c_str(), 100, 100, 100, msg);
		}
	}
}

void DumpData(Game* g) {
	// This function is for debug purposes only

	ofstream myfile;
	myfile.open("dump.txt");

	// Interfaces
	myfile << "|===============================|\n";
	myfile << "|========== INTERFACES =========|\n";
	myfile << "|===============================|\n";
	for (int i = 0; i < MAX_INTERFACES; i++) {
		auto inter = g->Interfaces[i];
		if (inter.sprite_id != -1) {
			myfile << ("Interface[" + to_string(i) + "] is used, with sprite id " + to_string(inter.sprite_id) + "\n");
		} else {
			myfile << ("Interface[" + to_string(i) + "] is EMPTY!\n");
		}
	}

	// Buttons
	myfile << "|===============================|\n";
	myfile << "|============ Buttons ==========|\n";
	myfile << "|===============================|\n";
	for (int i = 0; i < MAX_BUTTONS; i++) {
		auto but = g->Buttons[i];
		if (but.interface_id != -1) {
			myfile << ("Button[" + to_string(i) + "] is bound to Interface[" + to_string(but.interface_id) + "]\n");
		} else {
			myfile << ("Button[" + to_string(i) + "] is EMPTY!\n");
		}
	}
	myfile.close();
}

#define SYSTEM_COLOUR 139, 49, 121
void HandleCommand(Game* g, const char* msg) {
	if ((string)msg == "/version") {
		AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, ((string)"Running: " + ENGINE_VERSION).c_str());
	} else {
		string type;
		string command = msg;
		istringstream args(command);

		args >> type;
		if (type == "/help") {
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "List of usable commands:");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/help");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/grid");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/megamap");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/dump");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/day");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/night");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/timer <seconds (0 = RESET)> ");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/seed <i>");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/welcome");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/object <i>");
		} else if (type == "/object") {
			int object_type = -1;
			args >> object_type;
			if (object_type > 0 && object_type < MapObjectInfo::EMapObjectType::EMapObjectCount) {
				MapObjectInfo::MapObject mobj =
					MapObjectInfo::GenerateObjectByType((MapObjectInfo::EMapObjectType)object_type, g->pl.x, g->pl.y);
				g->map.Objects.push_back(mobj);
				g->map.OrganizeObjects();
				g->map.CreateSolids();
				AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, ("Spawned object " + to_string(object_type)).c_str());
			}
		} else if (type == "/seed") {
			args >> g->map.seed;
		} else if (type == "/timer") {
			args >> g->game_duration.seconds_start;
			g->game_duration.ticks = 0;
			if (g->game_duration.seconds_start == 0) {
				g->game_duration.inverted = false;
			} else {
				g->game_duration.inverted = true;
			}
		} else if (type == "/megamap") {
			g->debug.showMegaMap = !g->debug.showMegaMap;
		} else if (type == "/dump") {
			DumpData(g);
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "Dumped data to 'dump.txt'");
		} else if (type == "/day") {
			g->weather.StartDay(g);
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "Started day cycle.");
		} else if (type == "/night") {
			g->weather.StartNight(g);
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "Started night cycle.");
		} else if (type == "/grid") {
			g->debug.grid = !g->debug.grid;
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "Toggled grid.");
		} else if(type == "/welcome"){
			g->welcome_interface.ResetInterface(g);
			g->welcome_interface.SetDimensions((g->BWIDTH / 2) - 400, (g->BHEIGHT / 2) - 250, 800, 500);
			g->welcome_interface.LoadTextFromFile("dec/TEST.dec");
			g->welcome_interface.InterpretAllRawText();
			g->Interfaces[INTERFACE_WELCOME].visible = true;
		} else {
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, ("Unrecognized command: " + type).c_str());
		}
	}
}

void DrawBar(Game* g, SpriteStruct* sprites, BarType type, int x, int y, int length) {
	// Assign correct sprites according to bar type
	int sprite_left, sprite_center, sprite_right;
	switch (type) {
	case BarType::EMPTY:
		sprite_left = SPRITE_INTERFACE_BAR_EL;
		sprite_center = SPRITE_INTERFACE_BAR_EC;
		sprite_right = SPRITE_INTERFACE_BAR_ER;
		break;
	case BarType::RED:
		sprite_left = SPRITE_INTERFACE_BAR_RL;
		sprite_center = SPRITE_INTERFACE_BAR_RC;
		sprite_right = SPRITE_INTERFACE_BAR_RR;
		break;
	case BarType::YELLOW:
		sprite_left = SPRITE_INTERFACE_BAR_YL;
		sprite_center = SPRITE_INTERFACE_BAR_YC;
		sprite_right = SPRITE_INTERFACE_BAR_YR;
		break;
	case BarType::BLUE:
		sprite_left = SPRITE_INTERFACE_BAR_BL;
		sprite_center = SPRITE_INTERFACE_BAR_BC;
		sprite_right = SPRITE_INTERFACE_BAR_BR;
		break;
	default:
		// Empty
		sprite_left = SPRITE_INTERFACE_BAR_EL;
		sprite_center = SPRITE_INTERFACE_BAR_EC;
		sprite_right = SPRITE_INTERFACE_BAR_ER;
		break;
	}

	// Draw the bar
	DrawImage(g, sprites->img_interface[sprite_left], x, y, 0);
	int wi = length - 36;
	DrawScaledImage(g, sprites->img_interface[sprite_center], x + 9, y, wi, 0, 0);
	DrawImage(g, sprites->img_interface[sprite_right], x + (length - 9), y, 0);
}

void DrawInterfaceBox(Game* g, SpriteStruct* sprites, InterfaceBoxType type, int x, int y, int w, int h, float opacity) {
	// Subtract the corner sprites (they're 8x8)
	w -= 16;
	h -= 16;

	// Correct minimum sizes
	if (w < 1) w = 1;
	if (h < 1) h = 1;

	// Assign correct sprites and colours according to type
	int sprite_corner, sprite_hor, sprite_ver, re, gr, bl;
	switch (type) {
	case InterfaceBoxType::BROWN:
		sprite_corner = SPRITE_INTERFACE_BOX_BC;
		sprite_hor = SPRITE_INTERFACE_BOX_BH;
		sprite_ver = SPRITE_INTERFACE_BOX_BV;
		re = 151;
		gr = 113;
		bl = 74;
		break;
	default:
		// Default will be brown
		sprite_corner = SPRITE_INTERFACE_BOX_BC;
		sprite_hor = SPRITE_INTERFACE_BOX_BH;
		sprite_ver = SPRITE_INTERFACE_BOX_BV;
		re = 151;
		gr = 113;
		bl = 74;
		break;
	}
	/* ======================
	    Draw the interface box
		First draw 4 corners 
	   ====================== */
	// Top-left
	DrawImage(g, sprites->img_interface[sprite_corner],
		x,
		y,
		0, opacity);

	// Top-right
	DrawImage(g, sprites->img_interface[sprite_corner],
		x + 8 + w,
		y,
		ALLEGRO_FLIP_HORIZONTAL, opacity);

	// Bottom-left
	DrawImage(g, sprites->img_interface[sprite_corner],
		x,
		y + 8 + h,
		ALLEGRO_FLIP_VERTICAL, opacity);

	// Bottom-right
	DrawImage(g, sprites->img_interface[sprite_corner],
		x + 8 + w,
		y + 8 + h,
		ALLEGRO_FLIP_HORIZONTAL + ALLEGRO_FLIP_VERTICAL, opacity);

	/* ======================
		Then draw the sides
	   ====================== */
	// Top
	DrawScaledImage(g, sprites->img_interface[sprite_hor],
		x + 8,
		y,
		w - 1, 0, 0, opacity);

	// Bottom
	DrawScaledImage(g, sprites->img_interface[sprite_hor],
		x + 8,
		y + 8 + h,
		w - 1, 0, ALLEGRO_FLIP_VERTICAL, opacity);

	// Left
	DrawScaledImage(g, sprites->img_interface[sprite_ver],
		x,
		y + 8,
		0, h - 1, 0, opacity);

	// Right
	DrawScaledImage(g, sprites->img_interface[sprite_ver],
		x + 8 + w,
		y + 8,
		0, h - 1, ALLEGRO_FLIP_HORIZONTAL, opacity);

	/* ==========================
		Then fill middle colour
	============================= */
	DrawRectangle(g, x + 8, y + 8, w, h, re, gr, bl, opacity);
}