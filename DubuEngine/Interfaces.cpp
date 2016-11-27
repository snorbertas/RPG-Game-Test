#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <sstream>
#include "Game.h"
#include "GameHandler.h"
#include "ScaledDraw.h"
#include "Interfaces.h"

inline const char * const BoolToString(bool b)
{
	return b ? "Yes" : "No";
}

void LoadInterfaces(Game* g){
	g->Interfaces = new Interface[MAX_INTERFACES];
	g->Buttons = new Button[MAX_BUTTONS];
	for (int i = 0; i < MAX_INTERFACES; i++) {
		g->Interfaces[i].sprite_id = 5;
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

	// Interface 29 - Chat/Console
	NewInterface(&g->Interfaces[INTERFACE_CHAT], NO_SPRITE, 0, g->BHEIGHT - 480);

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
			DrawImage(g, img_interface[g->Interfaces[i].sprite_id],
				g->Interfaces[i].x,
				g->Interfaces[i].y,
				0, g->Interfaces[i].opacity);
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
						ALLEGRO_ALIGN_LEFT, "Jump:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.jump_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Action:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.crouch_bind));
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
						ALLEGRO_ALIGN_LEFT, "Inventory:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.inventory_bind));
					DrawText(font[2], 0, 0, 0,
						g->Interfaces[INTERFACE_KEYBOARD].x + 202,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + (offset += 25),
						ALLEGRO_ALIGN_LEFT, "Skills:");
					DrawText(font[2], 255, 255, 255,
						g->Interfaces[INTERFACE_KEYBOARD].x + 333,
						g->Interfaces[INTERFACE_KEYBOARD].y + 21 + offset,
						ALLEGRO_ALIGN_CENTER, GetKeyName(g->keys.skills_bind));
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
					DrawRectangle(g, 95, 300, 560, 240, 0, 0, 0, 0.1);
					DrawText(font[0], 255, 200, 200, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "[Programming]");
					DrawText(font[2], 0, 0, 0, credits_x, credits_y += 30, ALLEGRO_ALIGN_LEFT, "Norbert S.");
					DrawText(font[2], 0, 0, 50, credits_x, credits_y += 20, ALLEGRO_ALIGN_LEFT, "https://github.com/snorbertas");
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
			if(i != 6 && i != 2 && i != 7 && i != 31 && i != 32 && i != 33 && i != 29 && i != 27 && i != 28 && i != 34 && i != 35 && i != 26) top_i = i;
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

#define SYSTEM_COLOUR 139, 49, 121
void HandleCommand(Game* g, const char* msg) {
	if ((string)msg == "/version") {
		AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, ((string)"Running: " + ENGINE_VERSION).c_str());
	}
	else {
		string type;
		string command = msg;
		istringstream args(command);

		args >> type;
		if (type == "/help") {
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "List of usable commands:");
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, "/help");
		} else {
			AddChatMessage(g->chat, "__SYSTEM__", SYSTEM_COLOUR, ("Unrecognized command: " + type).c_str());
		}
	}
}