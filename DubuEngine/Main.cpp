//#pragma comment(linker, "/STACK:16777216")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define recv_s(Socket,DataPtr,Size,Other) recv( (Socket), (char *)(DataPtr), (Size), (Other))
#define send_s(Socket,DataPtr,Size,Other) send( (Socket), (char *)(DataPtr), (Size), (Other)) 
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "../DubuEngineServer/DEProtocol.h"

#include <process.h>
#include <fstream>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string.h>
#include <iostream>
#include "bass.h"
#include "DEEvents.h"
#include "Main.h"
#include "Game.h"
#include "PacketHandler.h"
#include "MenuRenderer.h"
#include "MenuHandler.h"
#include "GameHandler.h"
#include "GameRenderer.h"
#include "InputHandler.h"
#include "Interfaces.h"
#include "DEText.h"
#include <vector>
#include <array>
using namespace std;

int main(){
	// Initiate Allegro
	if(!al_init()){
		al_show_native_message_box(NULL, NULL, NULL, "failed to initialize allegro!", NULL, NULL);                   
		return -1;
	}

	// Initiate BASS
	if (BASS_Init(-1, 44100, 0, 0, NULL)) {
		menu_theme = BASS_SampleLoad(false, "music/theme_0.mp3", 0, 0, 1, BASS_SAMPLE_LOOP);
		main_channel = BASS_SampleGetChannel(menu_theme, false);
	} else {
		al_show_native_message_box(NULL, NULL, NULL, "failed to initialize Bass", NULL, NULL);
	}

	// Load settings
	LoadSettings();

	// Initiate display
	al_set_new_display_flags(g.window_mode);
	display = al_create_display(g.s_x, g.s_y);
	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "failed to initialize display!", NULL, NULL);
		return -1;
	}

	// Set scene to -1 (loading)
	g.scene = -1;

	// Set random seed
	srand(time(NULL));

	// Register event functions
	RegisterCameraEvents(TimerEvents, MouseEvents, KeyboardEvents);

	// Initiate allegro addons, registed mouse, keyboard, etc.
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();

	// Load Icon
	sprites.img_sprite[SPRITE_ICON] = al_load_bitmap(("sprites/sprite_" + to_string(SPRITE_ICON) + ".png").c_str());
	if (sprites.img_sprite[SPRITE_ICON] != NULL) {
		al_set_display_icon(display, sprites.img_sprite[SPRITE_ICON]);
		al_set_window_title(display, ENGINE_VERSION);
	} else {
		g.done = true;
		al_show_native_message_box(display, "Error", "Can't load icon", "Game will quit.", "Quit", 0);
	}

	// Load samples
	LoadSamples();

	// Load fonts
	LoadFonts();

	// Load/initilizae interfaces
	LoadInterfaces(&g);

	// Initiate client socket
	InitClient();

	// Initiate timer and register events
	timer = al_create_timer(1.0 / TPS);
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_hide_mouse_cursor(display);

	// Main Loop
	MainLoop();

	// Save settings
	SaveSettings();

	// Free memory once loop ends
	BASS_Free();
	DestroyBitmaps();
	DestroySamples();
	DestroyFonts();
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);
	return 0;
}

void LoadGame() {
	RenderLoading(&g, NULL, NULL, font);
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	LoadSprites(sprites.img_background, "bg", "bg_", MAX_BACKGROUND_SPRITES);
	LoadSprites(sprites.img_sprite, "sprites", "sprite_", MAX_MISC_SPRITES);
	LoadSprites(sprites.img_body, "sprites/body", "body_", MAX_BODY_SPRITES);
	LoadSprites(sprites.img_tile, "sprites/tiles", "tile_", MAX_TILE_SPRITES);
	LoadSprites(sprites.img_icon, "sprites/icons", "icon_", MAX_ICON_SPRITES);
	LoadSprites(sprites.img_interface, "sprites/interfaces", "interface_", MAX_INTERFACE_SPRITES);
	LoadSprites(sprites.img_button, "sprites/buttons", "button_", MAX_BUTTON_SPRITES);
	g.menu.bg_x = (-1500 + g.BWIDTH) + (1500 - g.BWIDTH) * (float)((float)(g.BWIDTH - g.pos_x) / (float)g.BWIDTH);
	g.menu.bg_y = (-938 + g.BHEIGHT) + (938 - g.BHEIGHT) * (float)((float)(g.BHEIGHT - g.pos_y) / (float)g.BHEIGHT);
	g.scene = 0;
}

void RecreateDisplay() {
	cout << "Recreating Display...\n";
	g.remake_display = false;
	int recreate = false;
	if (al_get_display_flags(display) != g.window_mode) {
		recreate = true;
	}
	
	/* Make sure everything is up to standard
	   1280x720 is minimum base res, anything bellow that will get downscaled*/
	if (g.BWIDTH < 1280 || g.BHEIGHT < 720) {
		g.BWIDTH = 1280;
		g.BHEIGHT = 720;
	}
	ALLEGRO_MONITOR_INFO monitor_info;
	al_get_monitor_info(0, &monitor_info);
	if (g.window_mode == ALLEGRO_WINDOWED) {
		if (g.s_x > monitor_info.x2) {
			g.s_x = monitor_info.x2;
			g.BWIDTH = monitor_info.x2;
			g.s_y = (int)((monitor_info.x2 / 16) * 9);
			g.BHEIGHT = (int)((monitor_info.x2 / 16) * 9);
		}
	}
	if (g.window_mode == ALLEGRO_FULLSCREEN_WINDOW) {
		g.s_x = monitor_info.x2;
		g.s_y = monitor_info.y2;
		g.BWIDTH = monitor_info.x2;
		g.BHEIGHT = monitor_info.y2;
	}
	if (g.window_mode == ALLEGRO_FULLSCREEN) {
		if (g.s_x > monitor_info.x2 || g.s_y > monitor_info.y2) {
			g.s_x = monitor_info.x2;
			g.s_y = monitor_info.y2;
			g.BWIDTH = monitor_info.x2;
			g.BHEIGHT = monitor_info.y2;
		}
	}

	// Recreate fonts according to new resolution
	DestroyFonts();
	LoadFonts();
	if (g.window_mode == ALLEGRO_WINDOWED && !recreate) {
		al_resize_display(display, g.s_x, g.s_y);
		al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
	} else if (g.window_mode == ALLEGRO_FULLSCREEN_WINDOW && !recreate) {
		al_resize_display(display, g.s_x, g.s_y);
		al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
	} else {
		// Prevent memory leaks
		DestroyBitmaps();
		DestroySamples();
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_destroy_event_queue(event_queue);
		
		// Create new display
		al_set_new_display_flags(g.window_mode);
		display = al_create_display(g.s_x, g.s_y);
		
		// Reload sprites
		LoadGame();

		// Load samples
		LoadSamples();

		// Load Interfaces
		LoadInterfaces(&g);

		// Update window information (icon/title)
		al_set_display_icon(display, sprites.img_sprite[SPRITE_ICON]);
		al_set_window_title(display, ENGINE_VERSION);

		timer = al_create_timer(1.0 / 120);
		event_queue = al_create_event_queue();
		al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_register_event_source(event_queue, al_get_mouse_event_source());
		al_register_event_source(event_queue, al_get_display_event_source(display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_start_timer(timer);
		al_hide_mouse_cursor(display);
	}
}

void MainLoop() {
	// Loading sprites, etc.
	LoadGame();
	
	// Start timer
	al_start_timer(timer);

	// Main while loop here
	while (!g.done) {
		if (g.remake_display) {
			RecreateDisplay();
		}
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN && g.window_mode == ALLEGRO_FULLSCREEN) {
			g.remake_display = true;
		}
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			g.done = true;
		}
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (g.scene != -1) {
				HandleInterfaces(&g);
				// Timer values
				g.old_time = g.new_time;
				g.new_time = al_get_time();

				if (g.logini.try_login == true) {
					g.logini.try_login = false;
					Connect(&g);
				}
			}
			if (g.scene == -1) {
				// Loading
			} else if (g.scene == 0) { // Menu
				HandleMenu(&g, sample_sfx);
				if(g.scene == 0) RenderMenu(&g, &sprites, font);
				if (g.intro_a == 0) RenderInterfaces(&g, &sprites, font);
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			} else if (g.scene == 1) { // Game
				// Tick
				for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
					if (TimerEvents[i] != NULL) {
						TimerEvents[i](&g, sample_sfx);
					}
				}
				HandleGame(&g, sample_sfx);
				RenderGame(&g, &sprites, font);
				RenderInterfaces(&g, &sprites, font);
				al_flip_display();
				al_clear_to_color(al_map_rgb(10, 10, 10));
			} else if (g.scene == 2) { // Pause
				RenderGame(&g, &sprites, font);
				RenderInterfaces(&g, &sprites, font);
				al_flip_display();
				al_clear_to_color(al_map_rgb(10, 10, 10));
			}
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
				if (MouseEvents[i] != NULL && g.allow_input == true) {
					MouseEvents[i](&g, ev.mouse.button, false, sample_sfx);
				}
			}
			if (g.allow_input) {
				if (ev.mouse.button == 1) {
					LeftClick(&g, false, sample_sfx);
				} else if (ev.mouse.button == 2) {
					RightClick(&g, false, sample_sfx);
				}
			}
			g.allow_input = true;
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
				if (MouseEvents[i] != NULL && g.allow_input == true) {
					MouseEvents[i](&g, ev.mouse.button, true, sample_sfx);
				}
			}
			if (g.allow_input) {
				if (ev.mouse.button == 1) {
					LeftClick(&g, true, sample_sfx);
				} else if (ev.mouse.button == 2) {
					RightClick(&g, true, sample_sfx);
				}
			}
			g.allow_input = true;
		} else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			float r_x = (float)g.BWIDTH / (float)g.s_x;
			float r_y = (float)g.BHEIGHT / (float)g.s_y;
			g.pos_x = (int)((r_x)*ev.mouse.x);
			g.pos_y = (int)((r_y)*ev.mouse.y);
			if (ev.mouse.dz > 0) {
				// Scroll down
			} else if (ev.mouse.dz < 0) {
				// Scroll up
			}
		} else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			for (int i = 0; i < MAX_EVENT_FUNCTIONS; i++) {
				if (KeyboardEvents[i] != NULL && g.allow_input == true) {
					KeyboardEvents[i](&g, ev.keyboard.keycode, false, sample_sfx);
				}
			}
			if(g.allow_input == true) KeyboardFunction(&g, ev.keyboard.keycode, false, sample_sfx, font);
			g.allow_input = true;
		} else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			KeyboardFunction(&g, ev.keyboard.keycode, true, sample_sfx);
		} else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			g.done = true;
		}
	}
}

int LoadSprites(ALLEGRO_BITMAP** destination, const char* location, const char* prefix, int max) {
	for (int i = 0; i < max; i++) {
		stringstream strs;
		//strs << "sprites/sprite_";
		strs << location;
		strs << "/";
		strs << prefix;
		strs << i;
		strs << ".png";
		string temp_str = strs.str();
		char const* n = temp_str.c_str();
		cout << n << "\n";
		destination[i] = al_load_bitmap(n);
		if (destination[i] == NULL) {
			cout << "Can't access file: " << n << "\n";
		}
	}
	return 0;
}

int LoadSamples(){
	cout << "Loading samples..." << "\n";
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);
	for(int i = 0; i < MAX_SAMPLES; i++){
		stringstream strs;
		strs << "sounds/sfx_";
		strs << i;
		strs << ".wav";
		string temp_str = strs.str();
		char const* n = temp_str.c_str();
		cout << n << "\n";
		sample_sfx[i] = al_load_sample(n);
		if (sample_sfx[i] == NULL) {
			cout << "Can't access file: " << n << "\n";
		}
	}
	return 0;
}

int LoadFonts(){
	cout << "Loading fonts..." << "\n";
	al_init_font_addon();
	al_init_ttf_addon();
	float r_y = (float)g.s_y/(float)g.BHEIGHT;
	font[0] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y*24), 0);
	font[1] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y*18), 0);
	font[2] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y*16), 0);
	font[3] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y*20), 0);
	font[4] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y*12), 0);
	font[5] = al_load_font("fonts/Exo2-Bold.ttf", 18, 0);				// Constant chat font (for calculations)
	font[6] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y * 32), 0);
	font[7] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y * 48), 0);
	font[8] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y * 64), 0);
	font[9] = al_load_font("fonts/Exo2-Bold.ttf", (int)(r_y * 14), 0);
	return 0;
}

void DestroyBitmaps(){
	cout << "Destroying bitmaps..." << "\n";
	for(int i = 0; i < MAX_MISC_SPRITES; i++){
		al_destroy_bitmap(sprites.img_sprite[i]);
	}
	for(int i = 0; i < MAX_BACKGROUND_SPRITES; i++){
		al_destroy_bitmap(sprites.img_background[i]);
	}
	for (int i = 0; i < MAX_BODY_SPRITES; i++) {
		al_destroy_bitmap(sprites.img_body[i]);
	}
	for (int i = 0; i < MAX_TILE_SPRITES; i++) {
		al_destroy_bitmap(sprites.img_tile[i]);
	}
	for (int i = 0; i < MAX_ICON_SPRITES; i++) {
		al_destroy_bitmap(sprites.img_icon[i]);
	}
	for (int i = 0; i < MAX_INTERFACE_SPRITES; i++) {
		al_destroy_bitmap(sprites.img_interface[i]);
	}
	for (int i = 0; i < MAX_BUTTON_SPRITES; i++) {
		al_destroy_bitmap(sprites.img_button[i]);
	}
}

void DestroySamples(){
	cout << "Destroying samples..." << "\n";
	for(int i = 0; i < MAX_SAMPLES; i++){		
		al_destroy_sample(sample_sfx[i]);
	}
}

void DestroyFonts(){	
	cout << "Destroying fonts..." << "\n";
	for(int i = 0; i < MAX_FONTS; i++){
		al_destroy_font(font[i]);
	}
}

void LoadSettings(){
	ifstream file("settings.ini");

	while (!file.eof() && file.is_open()) {
		string meme;
		getline(file, meme);
		SubStrings setting = SplitString(meme, '=');

		if (setting.items == 3) {
			string setting_type = setting.s[0];
			int s_val = atoi(trim(setting.s[2]).c_str());
			if (setting_type == "BASE_WIDTH") g.BWIDTH = s_val;
			if (setting_type == "BASE_HEIGHT") g.BHEIGHT = s_val;
			if (setting_type == "WINDOW_WIDTH") g.s_x = s_val;
			if (setting_type == "WINDOW_HEIGHT") g.s_y = s_val;
			if (setting_type == "WINDOW_MODE") g.window_mode = s_val;
			if (setting_type == ("KEY_LEFT")) g.keys.left_bind = s_val;
			if (setting_type == ("KEY_RIGHT")) g.keys.right_bind = s_val;
			if (setting_type == ("KEY_JUMP")) g.keys.jump_bind = s_val;
			if (setting_type == ("KEY_CROUCH")) g.keys.crouch_bind = s_val;
			if (setting_type == ("KEY_CAMERA")) g.keys.camera_bind = s_val;
			if (setting_type == ("KEY_INVENTORY")) g.keys.inventory_bind = s_val;
			if (setting_type == ("KEY_SKILLS")) g.keys.skills_bind = s_val;
			if (setting_type == ("KEY_CHAT")) g.keys.chat_bind = s_val;
		}
	}
	file.close();

	if (g.s_x > 1920 || g.s_x < 640) {
		g.s_x = g.BHEIGHT;
		g.s_y = g.BWIDTH;
	}
	if (g.s_y > 1080 || g.s_y < 360) {
		g.s_x = g.BHEIGHT;
		g.s_y = g.BWIDTH;
	}
	if (g.s_y != (int)((g.s_x / 16) * 9)) {
		g.s_y = (int)((g.s_x / 16) * 9);
	}
	if (g.window_mode != ALLEGRO_FULLSCREEN_WINDOW &&
		g.window_mode != ALLEGRO_WINDOWED &&
		g.window_mode != ALLEGRO_FULLSCREEN) g.window_mode = ALLEGRO_WINDOWED;
}

void SaveSettings(){
	using namespace std;
	cout << "Saving settings..." << "\n";
	ofstream myfile;
	myfile.open ("settings.ini");
	myfile << ("BASE_WIDTH = " + to_string(g.BWIDTH) + "\n");
	myfile << ("BASE_HEIGHT = " + to_string(g.BHEIGHT) + "\n");
	myfile << ("WINDOW_WIDTH = " + to_string(g.s_x) + "\n");
	myfile << ("WINDOW_HEIGHT = " + to_string(g.s_y) + "\n");
	myfile << ("WINDOW_MODE = " + to_string(g.window_mode) + "\n");
	myfile << ("KEY_LEFT = " + to_string(g.keys.left_bind) + "\n");
	myfile << ("KEY_RIGHT = " + to_string(g.keys.right_bind) + "\n");
	myfile << ("KEY_JUMP = " + to_string(g.keys.jump_bind) + "\n");
	myfile << ("KEY_CROUCH = " + to_string(g.keys.crouch_bind) + "\n");
	myfile << ("KEY_CAMERA = " + to_string(g.keys.camera_bind) + "\n");
	myfile << ("KEY_INVENTORY = " + to_string(g.keys.inventory_bind) + "\n");
	myfile << ("KEY_SKILLS = " + to_string(g.keys.skills_bind) + "\n");
	myfile << ("KEY_CHAT = " + to_string(g.keys.chat_bind) + "\n");
	myfile.close();
}

int InitClient() {
	int iResult = WSAStartup(MAKEWORD(2, 2), &WsaDat);

	if (iResult) {
		cout << "WSA startup failed\n";
		return 0;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		cout << "Invalid Socket\n";
		return 0;
	}
	return 1;
}

void Connect(Game* g) {
	_beginthreadex(0, 0, ClientThread, g, 0, 0);
}

unsigned int __stdcall ClientThread(void *data) {
	cout << "Connecting to server...\n";
	Game* g = (Game*)data;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(g->server_port);
	addr.sin_addr.S_un.S_addr = inet_addr(g->server_ip);

	int iResult = connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in));
	if (iResult) {
		cout << "Failed to connect: " << WSAGetLastError() << "\n";
		ShowMessage(g, "Failed to connect to server.", "");
		g->logini.logging_in = false;
		g->Buttons[247].visible = true;
		g->connected = false;
		g->scene = 0;
		return 0;
	}
	cout << "Successfully connected to " << g->server_ip << "\n";

	// Socket test
	PacketBuffer2* test = new PacketBuffer2(PACKET_TYPE_AUTHENTICATE);
	AddToBuffer(test->buffer_1, g->logini.username_input.c_str());
	AddToBuffer(test->buffer_2, g->logini.password_input.c_str());
	AddPacketToQueue(g, test);
	
	// Disable nagle
	int flag = 1;
	setsockopt(sock,		// Socket affected
		IPPROTO_TCP,		// Set option at TCP level
		TCP_NODELAY,		// Name of option
		(char *)&flag,		// The cast is historical cruft
		sizeof(int));		// Length of option value

	// Set to non-blocking mode
	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	// Our inheritated packet id (so we know the size)
	uint16_t rec_packet_deriv = -1;

	while (true) {
		// Chill for 1ms to not stress out the CPU core
		Sleep(1);
		int nReadBytes = recv_s(sock, &rec_packet_deriv, sizeof(uint16_t), 0);
		if (nReadBytes == SOCKET_ERROR){
			// Nothing yet
		} else {
			// Set socket to blocking mode
			iMode = 0;
			ioctlsocket(sock, FIONBIO, &iMode);

			// Read the next packet depending on the deriv ID
			switch (rec_packet_deriv) {
			case DEP_EMPTY:
				cout << "Empty packet received!\n";
				break;
			case DEP_DERIV_PING: {
				Packet p;
				nReadBytes = recv_s(sock, &p, GetPacketSize(rec_packet_deriv), 0);
				HandlePacket(g, &p);
				break;
			}
			case DEP_DERIV_1BUFF: {
				PacketBuffer1 p(DEP_EMPTY);
				nReadBytes = recv_s(sock, &p, GetPacketSize(rec_packet_deriv), 0);
				HandlePacket(g, &p);
				break;
			}
			case DEP_DERIV_2BUFF:
				PacketBuffer2 p(DEP_EMPTY);
				nReadBytes = recv_s(sock, &p, GetPacketSize(rec_packet_deriv), 0);
				HandlePacket(g, &p);
				break;
			}

			// Unblock our socket again
			iMode = 1;
			ioctlsocket(sock, FIONBIO, &iMode);
		}
		// Send packets
		if (g->ReadyToSendPackets) {
			g->ReadyToSendPackets = false;
			for (int i = 0; i < MAX_PACKETS_CLIENT; i++) {
				if (!g->PacketQueue[i].empty()) {
					// Send the packet deriv ID first
					uint16_t send_packet_deriv = g->PacketQueue[i].p()->deriv();
					int nSendBytes = send_s(sock, &send_packet_deriv, sizeof(uint16_t), 0);

					// Block the socket
					iMode = 0;
					ioctlsocket(sock, FIONBIO, &iMode);

					// Then send the actual packet
					if (send_packet_deriv != DEP_EMPTY) {
						nSendBytes = send_s(sock, g->PacketQueue[i].p(), GetPacketSize(send_packet_deriv), 0);

						// Ping
						if (g->PacketQueue[i].p()->deriv() == DEP_DERIV_PING) {
							g->time_sent_ping = al_get_time();
						}
					} else {
						cout << "Error: Can't send empty packet.\n";
					}

					// Unblock the socket
					iMode = 1;
					ioctlsocket(sock, FIONBIO, &iMode);

					g->PacketQueue[i].free();

					if (nSendBytes == SOCKET_ERROR) {
						cout << "Lost Connection to server\n";
						shutdown(sock, SD_SEND); // Shutdown our socket		
						closesocket(sock);		 // Close our socket entirely
						ShowMessage(g, "Lost connection to server.", "");
						g->scene = 0;
						HideAllInterfaces(g, INTERFACE_MESSAGE);
						g->Interfaces[INTERFACE_MAIN_MENU].visible = true;
						g->Buttons[247].visible = true;
						g->connected = false;

						//Re-innitiate the socket
						InitClient();
						return 0;
					}
				}
			}
		}
	}
	return 1;
}