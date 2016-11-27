#pragma once
#include <string>
#define MAX_CHAT_MESSAGES 21

// Chat message structure
struct Message {
	std::string message = " ";				// Message string
	std::string parent = " ";				// Sender string
	int r = 255;							// Red colour
	int g = 255;							// Green colour
	int b = 255;							// Blue colour
};

// Chat interface structure
struct Chat {
	int ghost = 0;							// If > 0, renders chat as a "ghost"
	bool show_chat = false;					// If true, chat is visible
	bool type_chat = false;					// If true, typing in chat is enabled
	int chat_indicator_count = 0;			//
	int backspace_timer = 0;				//	Timers
	int backspace_timer_wait = 60;			//
	std::string msg_input = "";				// Message input
	Message messages[MAX_CHAT_MESSAGES];	// Array of all messages (history)
};

/* ============================ AddChatMessage ============================
*		Adds a chat message to the chat log/interface on screen.
*
*		Called when a message is meant to be displayed on chat interface.
*/

void AddChatMessage(Chat& chat,
	const char* parent,					// The parent (sender) of message
	int re, int gr, int bl,				// Red, green, blue colour values
	const char* msg);					// The message