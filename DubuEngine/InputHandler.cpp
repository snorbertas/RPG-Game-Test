#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <fstream> 
#include <string.h>
#include <process.h>
#include <allegro5/allegro_native_dialog.h>
#include <Windows.h>
#include <sstream>
#include "Game.h"
#include "GameHandler.h"
#include "Interfaces.h"
#include "InputHandler.h"
#include "PlayerMovement.h"
#include "DEText.h"
#include "Digging.h"
#include "Peeing.h"
#include "Drinking.h"
#include "BoneHunt.h"
#include "BoneSweeper.h"

using namespace std;

void LeftClick(Game* g, bool release, ALLEGRO_SAMPLE** sample_sfx){
	if(!release){
		// Login Interface
		if (g->Interfaces[INTERFACE_LOGIN].visible == true) {
			if (MouseIsOn(g, g->Interfaces[INTERFACE_LOGIN].x + 106, g->Interfaces[INTERFACE_LOGIN].y + 16, 258, 32)) {
				g->logini.type_username = true;
			} else if (MouseIsOn(g, g->Interfaces[INTERFACE_LOGIN].x + 106, g->Interfaces[INTERFACE_LOGIN].y + 58, 258, 32)) {
				g->logini.type_username = false;
			}
		}
		
		// Drop-down menu cancelation
		if (g->Interfaces[4].visible && !MouseIsOn(g, g->Interfaces[4].x, g->Interfaces[4].y, 160, 80)) {
			g->Interfaces[4].visible = false;
		}
		if (g->Interfaces[5].visible && !MouseIsOn(g, g->Interfaces[5].x, g->Interfaces[5].y, 160, 60)) {
			g->Interfaces[5].visible = false;
		}

		// Volume adjustment engage
		if (g->Interfaces[3].visible) {
			// Music
			if (MouseIsOn(g, g->Interfaces[3].x + g->Buttons[29].x, g->Interfaces[3].y + g->Buttons[29].y, 160, 20)) {
				g->adjusting_music = true;
			}

			// Sound
			if (MouseIsOn(g, g->Interfaces[3].x + g->Buttons[30].x, g->Interfaces[3].y + g->Buttons[30].y, 160, 20)) {
				g->adjusting_sound = true;
			}
		}
	} else {
		// Interface Button's click
		bool done = false;

		// But first check for release of volume adjustment
		if (g->Interfaces[INTERFACE_OPTIONS].visible == true) {
			if (g->adjusting_music || g->adjusting_sound) {
				g->adjusting_music = false;
				g->adjusting_sound = false;
				done = true;
			}
		}

		// Loop trough all interfaces and buttons
		for(int i = 0; i < MAX_INTERFACES; i++){
			if(g->Interfaces[i].visible == true && done == false){
				for(int j = 0; j < MAX_BUTTONS; j++){
					if(g->Buttons[j].interface_id == i && InterfaceIsOnTop(g, i)){
						if(g->pos_x > g->Buttons[j].x+g->Interfaces[i].x &&
						g->pos_x < g->Buttons[j].x+g->Buttons[j].w+g->Interfaces[i].x &&
						g->pos_y > g->Buttons[j].y+g->Interfaces[i].y &&
						g->pos_y < g->Buttons[j].y+g->Buttons[j].h+g->Interfaces[i].y &&
						g->Buttons[j].visible){
							cout << "Clicked Button: " << j << "\n";
							switch(j){
								case 0: // New Game (Main Menu)
									//Fade(g, 0);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									g->Interfaces[INTERFACE_SINGLE_MODE_CHOICE].visible = true;
									done = true;
									break;
								case 1: // Options (Main Menu)
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									g->Interfaces[INTERFACE_OPTIONS].visible = true;
									done = true;
									break;
								case 2: // Quit (Main Menu)
									Fade(g, 0);
									g->menu.quitting = true;
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									done = true;
									break;
								case 3: // Play bone hunt
									g->Interfaces[INTERFACE_SINGLE_MODE_CHOICE].visible = false;
									g->Interfaces[INTERFACE_BONEHUNT_LEVEL_CHOICE].visible = true;
									done = true;
									break;
								case 4: // Play bonesweeper
									g->Interfaces[INTERFACE_BONEHUNT_LEVEL_CHOICE].visible = false;
									g->game_mode = GameMode::GM_BoneSweeper;
									_beginthreadex(0, 0, MapGenerationThread, g, 0, 0);
									done = true;
									break;
								case 5: // Resolution drop-down
									if(!g->Interfaces[4].visible && !g->Interfaces[5].visible){
										g->Interfaces[4].visible = true;
										g->Interfaces[5].visible = false;
										done = true;
									}
									break;		
								case 6: // Window mode drop-down
									if(!g->Interfaces[4].visible && !g->Interfaces[5].visible){
										g->Interfaces[5].visible = true;
										g->Interfaces[4].visible = false;
										done = true;
									}
									break;
								case 7: // Play survival
									done = true;
									break;
								case 8: // 1024x576
									g->Interfaces[4].visible = false;
									g->s_x = 1024;
									g->s_y = 576;
									g->BWIDTH = 1280;
									g->BHEIGHT = 720;
									g->remake_display = true;
									done = true;
									break;
								case 9: // 1280x720
									g->Interfaces[4].visible = false;
									g->s_x = 1280;
									g->s_y = 720;
									g->BWIDTH = 1280;
									g->BHEIGHT = 720;
									g->remake_display = true;
									done = true;
									break;
								case 10: // 1600x900
									g->Interfaces[4].visible = false;
									g->s_x = 1600;
									g->s_y = 900;
									g->BWIDTH = 1600;
									g->BHEIGHT = 900;
									g->remake_display = true;
									done = true;
									break;
								case 11: // 1920x1080
									g->s_x = 1920;
									g->s_y = 1080;
									g->BWIDTH = 1920;
									g->BHEIGHT = 1080;
									g->remake_display = true;
									g->Interfaces[4].visible = false;
									done = true;
									break;
								case 12:
									g->window_mode = ALLEGRO_WINDOWED;
									g->remake_display = true;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 13:
									g->window_mode = ALLEGRO_FULLSCREEN;
									g->remake_display = true;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 14:
									g->window_mode = ALLEGRO_FULLSCREEN_WINDOW;
									g->remake_display = true;
									g->Interfaces[5].visible = false;
									done = true;
									break;
								case 15: // Okay
									HideAllInterfaces(g, 0);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									done = true;
									break;
								case 16: // Okay (Message Interface)
									g->Interfaces[INTERFACE_MESSAGE].visible = false;
									g->Buttons[16].sprite_id = SPRITE_BUTTON_OKAY;
									g->binding_key = NULL;
									done = true;
									break;
								case 17: // Button 0 for welcome interface (Play)
									g->Interfaces[INTERFACE_WELCOME].visible = false;
									g->game_duration.ticks = 0;
									g->game_duration.seconds_start = 300;
									g->game_duration.inverted = true;
									g->game_duration.ticking = true;
									done = true;
									break;
								case 27: // Turn on/off shadow rendering
									g->weather.cycle.sunlight.active = !g->weather.cycle.sunlight.active;
									done = true;
									break;
								case 28: // Done (from options)
									if (g->scene == 2) {
										HideAllInterfaces(g, INTERFACE_PAUSE);
										g->Interfaces[INTERFACE_PAUSE].visible = true;
									} else {
										HideAllInterfaces(g, INTERFACE_MAIN_MENU);
										g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									}
									done = true;
									break;
								case 31: // Done (from game over)
									g->game_over = false;
									HideAllInterfaces(g, INTERFACE_MAIN_MENU);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									g->scene = 0;
									done = true;
									break;
								case 39:
									g->Interfaces[INTERFACE_SINGLE_MODE_CHOICE].visible = true;
									g->Interfaces[INTERFACE_BONEHUNT_LEVEL_CHOICE].visible = false;
									done = true;
									break;
								case 241:
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									g->Interfaces[INTERFACE_LOGIN].visible = true;
									done = true;
									break;
								case 242:
									g->Interfaces[INTERFACE_MAIN_MENU].visible = false;
									g->Interfaces[INTERFACE_CREDITS].visible = true;
									done = true;
									break;
								case 247:
									if (g->logini.username_input != "" && g->logini.password_input != "") {
										g->logini.logging_in = true;
										g->Buttons[247].visible = false;
										g->logini.try_login = true;
									}
									done = true;
									break;
								case 248:
									HideAllInterfaces(g, 3);
									g->Interfaces[INTERFACE_OPTIONS].visible = true;
									done = true;
									break;
								case 249:
									HideAllInterfaces(g, 9);
									g->Interfaces[INTERFACE_KEYBOARD].visible = true;
									done = true;
									break;
								case 250:
									RequestBindKeyInput(g, &g->keys.pee_bind);
									done = true;
									break;
								case 251:
									RequestBindKeyInput(g, &g->keys.drink_bind);
									done = true;
									break;
								case 252:
									RequestBindKeyInput(g, &g->keys.sprint_bind);
									done = true;
									break;
								case 258:
									RequestBindKeyInput(g, &g->keys.left_bind);
									done = true;
									break;
								case 259:
									RequestBindKeyInput(g, &g->keys.right_bind);
									done = true;
									break;
								case 260:
									RequestBindKeyInput(g, &g->keys.up_bind);
									done = true;
									break;
								case 261:
									RequestBindKeyInput(g, &g->keys.down_bind);
									done = true;
									break;
								case 262:
									HideAllInterfaces(g, INTERFACE_VERSION);
									g->Interfaces[INTERFACE_CHAT].visible = true;
									g->Interfaces[INTERFACE_RADAR].visible = true;
									g->Interfaces[INTERFACE_STATS].visible = true;
									g->Interfaces[INTERFACE_SCORE].visible = true;
									g->Interfaces[INTERFACE_MINI_MAP].visible = true;
									//
									g->scene = 1;
									done = true;
									break;
								case 263:
									g->Interfaces[INTERFACE_PAUSE].visible = false;
									g->Interfaces[INTERFACE_OPTIONS].visible = true;
									done = true;
									break;
								case 264:
									g->scene = 0;
									HideAllInterfaces(g, 0);
									g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
									g->Interfaces[6].visible = true;
									done = true;
									break;
								case 265:
									RequestBindKeyInput(g, &g->keys.camera_bind);
									done = true;
									break;
								case 266:
									RequestBindKeyInput(g, &g->keys.dig_bind);
									done = true;
									break;
								case 267:
									RequestBindKeyInput(g, &g->keys.sniff_bind);
									done = true;
									break;
								case 268:
									RequestBindKeyInput(g, &g->keys.chat_bind);
									done = true;
									break;
							}

							// For buttons that don't have switch cases
							if (g->Interfaces[INTERFACE_BONEHUNT_LEVEL_CHOICE].visible == true) {
								int clicked_level = j - 39;
								if (clicked_level >= 1 && clicked_level <= 30) {
									g->level = clicked_level;
									g->game_mode = GameMode::GM_BoneHunt;
									g->Interfaces[INTERFACE_BONEHUNT_LEVEL_CHOICE].visible = false;
									_beginthreadex(0, 0, MapGenerationThread, g, 0, 0);
									done = true;
								}
							}

							// End of click
						}
					}
				}
			}
		}
	}
}

void RightClick(Game* g, bool release, ALLEGRO_SAMPLE** sample_sfx){
	if(!release){
		if (g->Interfaces[4].visible) g->Interfaces[4].visible = false;
		if (g->Interfaces[5].visible) g->Interfaces[5].visible = false;
		if (g->scene == 1 && !g->game_over) g->mouse_pathing = true; 
	} else {
		g->mouse_pathing = false;
	}
}

void KeyboardFunction(Game* g, int kid, bool release,  ALLEGRO_SAMPLE** sample_sfx, ALLEGRO_FONT** font){
	bool can_type = true;
	if(!release){
		if (g->binding_key != NULL) {
			if (kid == ALLEGRO_KEY_ESCAPE || (kid >= ALLEGRO_KEY_F1 && kid <= ALLEGRO_KEY_F12)) {
				//
			} else {
				int* fix_key = KeyIsBound(g, kid);
				BindKey(g->binding_key, kid);
				g->Interfaces[INTERFACE_MESSAGE].visible = false;
				g->Buttons[16].sprite_id = SPRITE_BUTTON_OKAY;
				if (fix_key != NULL && fix_key != g->binding_key) *fix_key = -1;
				g->binding_key = NULL;
			}
		} else if (g->Interfaces[INTERFACE_LOGIN].visible == true && !g->logini.logging_in) {
			// Login interface typing
			if (g->logini.type_username) {
				int msg_lenght = strlen(g->logini.username_input.c_str());
				if (msg_lenght < 20) {
					g->logini.username_input = g->logini.username_input + GetCharacterFromKey(kid, g->keys.SHIFT);
				}
			} else {
				int msg_lenght = strlen(g->logini.password_input.c_str());
				if (msg_lenght < 20) {
					string old = g->logini.password_input;
					g->logini.password_input = g->logini.password_input + GetCharacterFromKey(kid, g->keys.SHIFT);
					if(old != g->logini.password_input)	g->logini.password_input_mask += "*";
				}
			}
			if (kid == ALLEGRO_KEY_TAB || kid == ALLEGRO_KEY_ENTER || kid == ALLEGRO_KEY_PAD_ENTER) {
				g->logini.type_username = !g->logini.type_username;
			}
			if (kid == ALLEGRO_KEY_BACKSPACE) {
				g->keys.backspace = true;
				if (g->logini.type_username == true) {
					if (g->logini.username_input != "") {
						g->logini.username_input.pop_back();
						g->logini.backspace_timer = 5;
					}
				} else {
					if (g->logini.password_input != "") {
						g->logini.password_input.pop_back();
						g->logini.password_input_mask.pop_back();
						g->logini.backspace_timer = 5;
					}
				}
			}
			if (kid == ALLEGRO_KEY_LSHIFT || kid == ALLEGRO_KEY_RSHIFT) {
				g->keys.SHIFT = true;
			}
		} else if (g->scene == 1) {
			if (!g->chat.type_chat) {
				if(HandlePlayerMovementKeyBinds(g, kid)){
					// If true, there was a movement key bind handled
					g->mouse_pathing = false;
				} else if (kid == g->keys.dig_bind) {
					if (!g->pl.digging && !g->pl.peeing && !g->pl.drinking) {
						StartDigging(g, &g->pl);
					}
				} else if (kid == g->keys.sprint_bind) {
						g->keys.sprint = true;
				} else if (kid == g->keys.pee_bind) {
					if (!g->pl.digging && !g->pl.peeing && !g->pl.drinking) {
						if (g->pl.pee_ammo == 0) {
							AddChatMessage(g->chat, "__SYSTEM__", 139, 49, 121, "Empty Bladder");
						} else {
							StartPeeing(g, &g->pl);
						}
					}
				} else if (kid == g->keys.drink_bind) {
					if (!g->pl.digging && !g->pl.peeing && !g->pl.drinking) {
						StartDrinking(g, &g->pl);
					}
				} else if (kid == g->keys.sniff_bind) {
					if (g->radar.mode == Radar::Mode::DIRECTION_SNIFF) {
						g->radar.mode = Radar::Mode::DISTANCE_SNIFF;
					} else {
						g->radar.mode = Radar::Mode::DIRECTION_SNIFF;
					}
				} else if (kid == g->keys.camera_bind) {
					g->camera.mode = CAMERA_MODE_LOCKED;
				} else if (kid == g->keys.chat_bind) {
					g->chat.show_chat = true;
					if (!g->chat.type_chat) {
						g->chat.type_chat = true;
					}
					can_type = false;
				}
			}
			if (g->chat.type_chat && can_type) {
				int msg_lenght = al_get_text_width(font[5], (g->pl.name.c_str() + g->chat.msg_input).c_str());
				if (msg_lenght < 480) {
					g->chat.msg_input = g->chat.msg_input + GetCharacterFromKey(kid, g->keys.SHIFT);
				}
				if (kid == ALLEGRO_KEY_BACKSPACE) {
					g->keys.backspace = true;
					if (g->chat.type_chat && g->chat.show_chat && g->chat.backspace_timer <= 0) {
						if (g->chat.msg_input != "") {
							g->chat.msg_input.pop_back();
						}
					}
				}
				if (kid == ALLEGRO_KEY_ENTER || kid == ALLEGRO_KEY_PAD_ENTER) {
					g->chat.show_chat = true;
					if (!g->chat.type_chat) {
						g->chat.type_chat = true;
					} else {
						g->chat.type_chat = false;
						g->chat.show_chat = false;
						if (g->chat.msg_input != "") {
							SubmitMessage(g, g->chat.msg_input.c_str());
							g->chat.msg_input.clear();
						}
					}
				}
				if (kid == ALLEGRO_KEY_LSHIFT || kid == ALLEGRO_KEY_RSHIFT) {
					g->keys.SHIFT = true;
				}
			} else {
				switch (kid) {
				case ALLEGRO_KEY_TAB:
					//g->chat.show_chat = true;
					g->score.show_score = true;
					break;
				case ALLEGRO_KEY_LCTRL:
					g->keys.CTRL = true;
					break;
				}
			}
		}
	} else {
		// Released key binds
		if (kid == g->keys.left_bind) {
			g->keys.left = false;
		}
		if (kid == g->keys.right_bind) {
			g->keys.right = false;
		}
		if (kid == g->keys.up_bind) {
			g->keys.up = false;
		}
		if (kid == g->keys.down_bind) {
			g->keys.down = false;
		}
		if (kid == g->keys.sprint_bind) {
			g->keys.sprint = false;
		}
		if (kid == ALLEGRO_KEY_LSHIFT || kid == ALLEGRO_KEY_RSHIFT) {
			g->keys.SHIFT = false;
		}
		if (kid == ALLEGRO_KEY_BACKSPACE && g->Interfaces[INTERFACE_LOGIN].visible) {
			g->keys.backspace = false;
			g->logini.backspace_timer_wait = 30;
		}
		if (g->scene == 1) {
			switch (kid) {
				case ALLEGRO_KEY_BACKSPACE:
					g->keys.backspace = false;
					g->chat.backspace_timer_wait = 30;
					break;
				case ALLEGRO_KEY_TAB:
					/*if (!g->chat.type_chat) {
						g->chat.show_chat = false;
						g->chat.ghost = 0;
					}*/
					g->score.show_score = false;
					break;
				case ALLEGRO_KEY_LCTRL:
					g->keys.CTRL = false;
					break;
			}
		}
		if(kid == ALLEGRO_KEY_ESCAPE && !g->logini.logging_in && g->binding_key == NULL) {
			if (g->chat.type_chat) {
				g->chat.type_chat = false;
				g->chat.show_chat = false;
			} else if (g->Interfaces[INTERFACE_PAUSE].visible){
				HideAllInterfaces(g, 4);
				g->Interfaces[INTERFACE_CHAT].visible = true;
				//
				g->scene = 1;
			} else if(g->scene == 1 || g->scene == 2){
				g->scene = 2;
				HideAllInterfaces(g, 10);
				g->Interfaces[INTERFACE_PAUSE].visible = true;
				g->Interfaces[6].visible = true;
				g->Interfaces[INTERFACE_CHAT].x = 0;
				g->Interfaces[INTERFACE_CHAT].y = g->BHEIGHT - 496;
			} else {
				g->scene = 0;
				HideAllInterfaces(g, 0);
				g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
			}
		}
	}
}

const char* GetCharacterFromKey(int key_id, bool shift) {
	if (shift) { // +SHIFT
		// Letters
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

		// Special
		if (key_id == ALLEGRO_KEY_TILDE) return "~";
		if (key_id == ALLEGRO_KEY_1) return "!";
		if (key_id == ALLEGRO_KEY_2) return "@";
		if (key_id == ALLEGRO_KEY_3) return "#";
		if (key_id == ALLEGRO_KEY_4) return "$";
		if (key_id == ALLEGRO_KEY_5) return "%";
		if (key_id == ALLEGRO_KEY_6) return "^";
		if (key_id == ALLEGRO_KEY_7) return "&";
		if (key_id == ALLEGRO_KEY_8) return "*";
		if (key_id == ALLEGRO_KEY_9) return "(";
		if (key_id == ALLEGRO_KEY_0) return ")";
		if (key_id == ALLEGRO_KEY_MINUS) return "_";
		if (key_id == ALLEGRO_KEY_EQUALS) return "+";
		if (key_id == ALLEGRO_KEY_OPENBRACE) return "{";
		if (key_id == ALLEGRO_KEY_CLOSEBRACE) return "}";
		if (key_id == ALLEGRO_KEY_SEMICOLON) return ":";
		if (key_id == ALLEGRO_KEY_QUOTE) return "\"";
		if (key_id == ALLEGRO_KEY_COMMA) return "<";
		if (key_id == ALLEGRO_KEY_FULLSTOP) return ">";
		if (key_id == ALLEGRO_KEY_SLASH) return "?";
	} else { // NO SHIFT
		// Letters
		if (key_id == ALLEGRO_KEY_Q) return "q";
		if (key_id == ALLEGRO_KEY_W) return "w";
		if (key_id == ALLEGRO_KEY_E) return "e";
		if (key_id == ALLEGRO_KEY_R) return "r";
		if (key_id == ALLEGRO_KEY_T) return "t";
		if (key_id == ALLEGRO_KEY_Y) return "y";
		if (key_id == ALLEGRO_KEY_U) return "u";
		if (key_id == ALLEGRO_KEY_I) return "i";
		if (key_id == ALLEGRO_KEY_O) return "o";
		if (key_id == ALLEGRO_KEY_P) return "p";
		if (key_id == ALLEGRO_KEY_A) return "a";
		if (key_id == ALLEGRO_KEY_S) return "s";
		if (key_id == ALLEGRO_KEY_D) return "d";
		if (key_id == ALLEGRO_KEY_F) return "f";
		if (key_id == ALLEGRO_KEY_G) return "g";
		if (key_id == ALLEGRO_KEY_H) return "h";
		if (key_id == ALLEGRO_KEY_J) return "j";
		if (key_id == ALLEGRO_KEY_K) return "k";
		if (key_id == ALLEGRO_KEY_L) return "l";
		if (key_id == ALLEGRO_KEY_Z) return "z";
		if (key_id == ALLEGRO_KEY_X) return "x";
		if (key_id == ALLEGRO_KEY_C) return "c";
		if (key_id == ALLEGRO_KEY_V) return "v";
		if (key_id == ALLEGRO_KEY_B) return "b";
		if (key_id == ALLEGRO_KEY_N) return "n";
		if (key_id == ALLEGRO_KEY_M) return "m";

		// Special
		if (key_id == ALLEGRO_KEY_TILDE) return "`";
		if (key_id == ALLEGRO_KEY_1) return "1";
		if (key_id == ALLEGRO_KEY_2) return "2";
		if (key_id == ALLEGRO_KEY_3) return "3";
		if (key_id == ALLEGRO_KEY_4) return "4";
		if (key_id == ALLEGRO_KEY_5) return "5";
		if (key_id == ALLEGRO_KEY_6) return "6";
		if (key_id == ALLEGRO_KEY_7) return "7";
		if (key_id == ALLEGRO_KEY_8) return "8";
		if (key_id == ALLEGRO_KEY_9) return "9";
		if (key_id == ALLEGRO_KEY_0) return "0";
		if (key_id == ALLEGRO_KEY_MINUS) return "-";
		if (key_id == ALLEGRO_KEY_EQUALS) return "=";
		if (key_id == ALLEGRO_KEY_OPENBRACE) return "[";
		if (key_id == ALLEGRO_KEY_CLOSEBRACE) return "]";
		if (key_id == ALLEGRO_KEY_SEMICOLON) return ";";
		if (key_id == ALLEGRO_KEY_QUOTE) return "'";
		if (key_id == ALLEGRO_KEY_COMMA) return ",";
		if (key_id == ALLEGRO_KEY_FULLSTOP) return ".";
		if (key_id == ALLEGRO_KEY_SLASH) return "/";
	}
	// Not capitalizable
	if (key_id == ALLEGRO_KEY_SPACE) return " ";
	if (key_id == ALLEGRO_KEY_PAD_0) return "0";
	if (key_id == ALLEGRO_KEY_PAD_1) return "1";
	if (key_id == ALLEGRO_KEY_PAD_2) return "2";
	if (key_id == ALLEGRO_KEY_PAD_3) return "3";
	if (key_id == ALLEGRO_KEY_PAD_4) return "4";
	if (key_id == ALLEGRO_KEY_PAD_5) return "5";
	if (key_id == ALLEGRO_KEY_PAD_6) return "6";
	if (key_id == ALLEGRO_KEY_PAD_7) return "7";
	if (key_id == ALLEGRO_KEY_PAD_8) return "8";
	if (key_id == ALLEGRO_KEY_PAD_9) return "9";
	if (key_id == ALLEGRO_KEY_PAD_SLASH) return "/";
	if (key_id == ALLEGRO_KEY_PAD_ASTERISK) return "*";
	if (key_id == ALLEGRO_KEY_PAD_MINUS) return "-";
	if (key_id == ALLEGRO_KEY_PAD_PLUS) return "+";
	if (key_id == ALLEGRO_KEY_PAD_DELETE) return ".";

	return "";
}

void ToClipboard(const string &s) {
	OpenClipboard(NULL);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

string FromClipboard(){
	OpenClipboard(NULL);
	HANDLE hData = GetClipboardData(CF_TEXT);
	char * pszText = static_cast<char*>(GlobalLock(hData));
	string text;
	if (pszText != nullptr) {
		text = pszText;
	} else {
		text = "null";
	}
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}

void RequestBindKeyInput(Game* g, int* target_bind) {
	g->binding_key = target_bind;
	ShowMessage(g, "Press the desired key", "");
	g->Buttons[16].sprite_id = SPRITE_BUTTON_CANCEL;
}

void BindKey(int* target, int key_id) {
	*target = key_id;
}

int* KeyIsBound(Game* g, int key_id) {
	if (g->keys.left_bind == key_id) return &g->keys.left_bind;
	if (g->keys.right_bind == key_id) return &g->keys.right_bind;
	if (g->keys.up_bind == key_id) return &g->keys.up_bind;
	if (g->keys.down_bind == key_id) return &g->keys.down_bind;
	if (g->keys.dig_bind == key_id) return &g->keys.dig_bind;
	if (g->keys.sniff_bind == key_id) return &g->keys.sniff_bind;
	if (g->keys.camera_bind == key_id) return &g->keys.camera_bind;
	if (g->keys.chat_bind == key_id) return &g->keys.chat_bind;
	return NULL;
}

unsigned int __stdcall MapGenerationThread(void *data) {
	Game* g = (Game*)data;

	// Start loading screen
	HideAllInterfaces(g, INTERFACE_GENERATING);
	g->Interfaces[INTERFACE_GENERATING].visible = true;
	srand(al_get_time() * 1000);
	g->menu.tip_id = rand() % 2;

	// Record time
	double current_time = al_get_time();

	// Generate map
	if (g->game_mode == GameMode::GM_BoneHunt) {
		g->map.seed = BoneHuntSeedAndTrim(g->level).first;
		g->map.SetTrim(BoneHuntSeedAndTrim(g->level).second);
	} else if (g->game_mode == GameMode::GM_BoneSweeper){
		g->map.seed = rand() % ((2 ^ 32) - 1);
	}
	g->map.ChangeForestMode(1);
	g->map.Generate(1);

	// Bonesweeper
	if (g->game_mode == GameMode::GM_BoneSweeper) {
		SpawnRandomMines(g, 500);
		CalculateRealBoneSweeper(g);
	}

	// Record new time
	double new_time = al_get_time();

	// Delay until 4.5 seconds if needed
	double const delay = 4.5;
	double taken_time = new_time - current_time;
	cout << "Seconds taken to generate map: " << taken_time << endl;
	if (taken_time < delay) {
		Sleep((delay - taken_time) * 1000);
	}

	// Fade out
	Fade(g, 0);
	g->menu.done_loading = true;
	g->menu.ticks_since_done_loading = 0;
	Sleep(2000);

	// Butterfly spawn example
	g->map.Butterflies.empty();
	g->map.PopulateButterflies(50);

	// Set everything up
	NewGame(g);
	g->weather = Weather(Weather::CloudMode::MODE_GAME);
	g->pl.x = 3200;
	g->pl.y = 3200;
	HideAllInterfaces(g, INTERFACE_CHAT);
	g->Interfaces[INTERFACE_VERSION].visible = true;
	g->Interfaces[INTERFACE_CHAT].visible = true;
	g->Interfaces[INTERFACE_RADAR].visible = true;
	g->Interfaces[INTERFACE_STATS].visible = true;
	g->Interfaces[INTERFACE_SCORE].visible = true;
	g->Interfaces[INTERFACE_MINI_MAP].visible = true;
	g->game_duration.ticking = false;
	g->game_duration.ticks = 0;
	g->game_duration.minutes = 5;
	g->game_duration.seconds = 0;
	g->game_duration.seconds_start = 300;

	// Welcome interface
	g->welcome_interface.ResetInterface(g);
	g->welcome_interface.SetDimensions((g->BWIDTH / 2) - 400, (g->BHEIGHT / 2) - 250, 800, 500);
	if (g->game_mode == GameMode::GM_BoneHunt) {
		g->welcome_interface.LoadTextFromFile("dec/BoneHunt.dec");
	} else if(g->game_mode == GameMode::GM_BoneSweeper) {
		g->welcome_interface.LoadTextFromFile("dec/BoneSweeper.dec");
	}
	g->welcome_interface.InterpretAllRawText();
	g->Interfaces[INTERFACE_WELCOME].visible = true;


	// Reset, fade and done :)
	g->menu.done_loading = false;
	Fade(g, 1);
	return 1;
}