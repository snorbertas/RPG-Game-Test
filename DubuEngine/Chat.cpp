#include "Chat.h"
#include "GameHandler.h"
#include <string>

void AddChatMessage(Chat& chat, const char* parent, int re, int gr, int bl, const char* msg) {
	chat.ghost = SecondsToTicks(5);
	for (int i = MAX_CHAT_MESSAGES - 2; i >= 0; i--) {
		chat.messages[i + 1].message = chat.messages[i].message;
		chat.messages[i + 1].parent = chat.messages[i].parent;
		chat.messages[i + 1].r = chat.messages[i].r;
		chat.messages[i + 1].g = chat.messages[i].g;
		chat.messages[i + 1].b = chat.messages[i].b;
	}
	if ((string)parent == "__SYSTEM__") {
		chat.messages[0].parent = msg;
		chat.messages[0].message = "";
		chat.messages[0].r = re;
		chat.messages[0].g = gr;
		chat.messages[0].b = bl;
	} else {
		chat.messages[0].parent = ((string)parent + ": ").c_str();
		chat.messages[0].message = msg;
		chat.messages[0].r = re;
		chat.messages[0].g = gr;
		chat.messages[0].b = bl;
	}
}