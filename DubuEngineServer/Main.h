#pragma once

/*	Main variables/functions. Don't see the point to put them in a header
	but whatever.  */

int port;											// Port for server to listen
bool done = false;									// Wether server is done running
ALLEGRO_EVENT_QUEUE *event_queue = NULL;			// Allegro event queue (for timer)
ALLEGRO_TIMER *timer = NULL;						// Allegro timer
WSADATA WsaDat;										// WSA Data for server
SOCKET Socket, client;								// The main socket accepting clients
SOCKADDR_IN serverInf;								// Server info
bool InitServer();									// Initiate server (returns false on failure)
unsigned int __stdcall ClientThread(void *data);	// Client Thread (multiple)
unsigned int __stdcall GameThread(void *data);		// Game Thread (single, unless multiple "rooms" are desired)
int NewPlayerID(Game*);								// Returns an avaible ID for new client/player connected
char* GetIP(SOCKET Client);							// Returns the IP address for a connected client
void LoadSettings();								// Loads server settings