#pragma once

#include "_EngineConfig.h"
#define TPS DE_CONFIG_TPS
#define ENGINE_VERSION DE_CONFIG_GAME

#include "DEDefinitions.h"
#include "../DubuEngineServer/DEProtocol.h"
#include "Sprites.h"
#include "Chat.h"
#include "Camera.h"
#include "Player.h"
#include <allegro5/allegro.h>
#include <iostream>

// Login interface structure
	struct LoginInterface {
		bool try_login = false;					// If true, client will attempt to connect
		bool logging_in = false;				// If true, means client is trying to connect
		int loading_angle = 0;					// Loading animation angle
		bool type_username = true;				// True if typing username. If false, typing password
		int type_indicator_count = 0;			//
		int backspace_timer = 0;				//	Timers
		int backspace_timer_wait = 60;			//
		string username_input = "";		// Username input
		string password_input = "";		// Password input
		string password_input_mask = "";	// Password input converted into starts ****
	};

// Keys strucutre (key bindings and input)
	struct Keys {
		int left_bind = ALLEGRO_KEY_LEFT;		// Binded key to move left
		int right_bind = ALLEGRO_KEY_RIGHT;		// Binded key to move right
		int jump_bind = ALLEGRO_KEY_UP;			// Binded key to jump
		int crouch_bind = ALLEGRO_KEY_DOWN;		// Binded key to crouch
		int camera_bind = ALLEGRO_KEY_Y;		// Binded key to change camera mode
		int inventory_bind = ALLEGRO_KEY_I;		// Binded key to open inventory
		int skills_bind = ALLEGRO_KEY_O;		// Binded key to open skills
		int chat_bind = ALLEGRO_KEY_ENTER;		// Binded key to chat/type
		bool left = false;						// If true, then moving left key is inputted
		bool right = false;						// If true, then moving right key is inputted
		bool down = false;						// If true, then crouch key is inputted
		bool jump = false;						// If true, then jump key is inputted
		bool CTRL = false;						// If true, then CTRL is being held down
		bool SHIFT = false;						// If true, then SHIFT is being held down
		bool backspace = false;					// If true, then Backspace is being held down
	};

// Menu structure
	struct Menu {
		bool quitting = false;					// Quitting (fade/animations)
		float bg_x = -20;						// Background x position
		float bg_y = -20;						// Background y position
		float bg_x_vel = 0.2;					// Background horizontal velocity
		float bg_y_vel = 0.3;					// Background vertical velocity
	};


// Button structure
	struct Button {
		bool visible = true;					// If true, button is rendered and active
		int sprite_id = 5;						// Sprite ID for button (sprite_id + 1 is highlighted when mouse is colliding)
		int w, h;								// Width and height for button (hitbox)
		int interface_id;						// Interface ID the button belongs to
		int x = -1;								// x position for button relative to interface
		int y = -1;								// y position for button relative to interface
		float opacity = 1;						// Button transperency (<1 is transperent)
	};

// Interface structure
	struct Interface {
		int sprite_id;							// Sprite ID for interface
		int x, y;								// x, y coordinates
		bool visible;							// If true, interface is visible and active
		float opacity = 1.0;					// Transperency
	};

/* =============================== Game Class =============================
 *		The big, main, scary Game class. Used as the primary object.
 *		This whole game code is orientated around the Game class.
 *
 *		Used everywhere.
 */
	class Game
	{
		public:
			// Constructor
			Game();

			// Socket-related variables
			int ticks_since_last_ping = 0;		// Used to calculate ping
			double time_sent_ping = 0;			// Used to calculate ping
			int ping = 0;						// Time taken to hear a response to last ping
			bool ReadyToSendPackets = false;	// If true, the main socket thread will send all packets
			bool connected = false;				// If true, socket is succesfully connected to the server
			PacketQ PacketQueue[MAX_PACKETS_CLIENT]; // A packet queue to store unsent packets

			// Non-socket related variables
			int fade_in = 0;					// Fade-in timer
			int fade_out = 0;					// Fade-out timer
			double old_time = 0;				// Last tick
			double new_time = 0;				// New tick
	/*?*/	int animation_tick = 0;				// Animation timer
			Camera camera;						// Camera object
			float music_volume;					// Music volume
			float music_volume_cd;				// Music volume cooldown
			bool remake_display;				// If true, the display will be recreated next tick
			int s_x = 1280;						// Scale to x
			int s_y = 720;						// Scale to y
			int BWIDTH = 1600;					// Base width
			int BHEIGHT = 900;					// Base height
			int scene;							// Scene ID
			float intro_a = 255;				// Intro alpha
			bool done;							// If true, the main loop will exit and exit program
			int pos_x = 0;						// Cursor x position
			int pos_y = 0;						// Cursor y position
			int menu_x = 0;						// Virtual cursor for menu
			int menu_y = 0;						// Virtual cursor for menu
			const char* Message1 = "Line1";		// Top (first) line message
			const char* Message2 = "Line2";		// Bottom (second) line message
			int* binding_key = NULL;			// Pointer to current key being binded
			int window_mode;					// Window mode
			const char* server_ip = DE_CONFIG_IP;	// Server IP address
			int server_port = 25565;			// Server port
			bool gameover = false;				// If true, the game is over, different handling
			bool allow_input = true;			// Wether input is allowed or not
			Chat chat;							// Chat object
			LoginInterface logini;				// Login Interface
			Button* Buttons;					// Pointer to array of buttons
			Interface* Interfaces;				// Pointer to array of interfaces
			Menu menu;							// Menu object
			Keys keys;							// Keys struct, holds the information on some input
			Player pl;
	};