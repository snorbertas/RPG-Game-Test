#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <iostream>
#include <fstream>
#include <sstream>
#define recv_s(Socket,DataPtr,Size,Other) recv( (Socket), (char *)(DataPtr), (Size), (Other))
#define send_s(Socket,DataPtr,Size,Other) send( (Socket), (char *)(DataPtr), (Size), (Other)) 
#include <winsock2.h>
#include <string>
#include <process.h>
#include <allegro5/allegro.h>
#include "Game.h"
#include "GameHandler.h"
#include "Main.h"
#include "Player.h"
#include "DEProtocol.h"
#include "PacketHandler.h"
using namespace std;

int main()
{
	LoadSettings();
	if (InitServer() == false) {
		return 0; // Can't initiate
	}

	Game g = Game();
	_beginthreadex(0, 0, GameThread, &g, 0, 0);

	while (!g.ready) { // Wait until game finishes initiliazing/loading
		Sleep(1000);
	}

	cout << "Listening for connections on port " << port << "\n";
	while (!g.done) {
		if (NewPlayerID(&g) >= 0) { // If server is not full
			client = accept(Socket, 0, 0);

			if (client == INVALID_SOCKET) {
				cout << "invalid client socket\n";
				continue;
			}
			if (NewPlayerID(&g) >= 0) {
				_beginthreadex(0, 0, ClientThread, &g, 0, 0);
			}
			else {
				cout << "New connection refused: SERVER FULL\n";
			}
		}
	}

	WSACleanup();
	return 0;
}

bool InitServer() {
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		cout << "WSA Initialization failed!\r\n";
		WSACleanup();
		return false;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		cout << "Socket creation failed.\r\n";
		WSACleanup();
		return false;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		cout << "Socket creation failed.\r\n";
		WSACleanup();
		return false;
	}

	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = INADDR_ANY;
	serverInf.sin_port = htons(25565);

	if (bind(Socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
	{
		cout << "Unable to bind socket!\r\n";
		WSACleanup();
		return false;
	}

	int iResult = listen(Socket, SOMAXCONN);

	if (iResult)
	{
		printf("Can't listen %u", GetLastError());
		return false;
	}
	return true;
}

unsigned int __stdcall GameThread(void *data)
{
	cout << "Initializing Game...\n";
	Game* g = (Game*)data;

	if (!al_init()) {
		cout << "Couldn't initiate Allegro Timer\n";
		return -1;
	}
	timer = al_create_timer(1.0 / 1.0);
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	g->ready = true;

	while (!g->done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			g->ready = true;
			HandleGame(g);
		}
	}
	return 0;
}

int NewPlayerID(Game* g) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (g->SocketUsed[i] == false) return i;
	}
	return -1;
}

unsigned int __stdcall ClientThread(void *data)
{
	Game* g = (Game*)data;
	int pID = NewPlayerID(g);
	g->ClientSocket[pID] = client;
	g->SocketUsed[pID] = true;
	SOCKET Client = g->ClientSocket[pID];

	cout << "Client connected (" << pID << ") " << GetIP(Client) << "\n";

	// Disable nagle
	int flag = 1;
	setsockopt(Client,	// Socket affected
		IPPROTO_TCP,	// Set option at TCP level
		TCP_NODELAY,    // Name of option
		(char *)&flag,  // The cast is historical cruft
		sizeof(int));   // Length of option value

	// Set to non-blocking mode
	u_long iMode = 1;
	ioctlsocket(Client, FIONBIO, &iMode);

	// Our inheritated packet id (so we know the size)
	uint16_t rec_packet_deriv = -1;

	while (true)
	{
		Sleep(1);
		int nReadBytes = recv_s(Client, &rec_packet_deriv, sizeof(uint16_t), 0);
		if (nReadBytes == SOCKET_ERROR) {
			// Nothing yet
		} else {
			// Set socket to blocking mode
			iMode = 0;
			ioctlsocket(Client, FIONBIO, &iMode);

			// Read the next packet depending on the deriv ID
			switch (rec_packet_deriv) {
			case DEP_EMPTY:
				cout << "Empty packet received!\n";
				break;
			case DEP_DERIV_PING: {
				Packet p;
				nReadBytes = recv_s(Client, &p, GetPacketSize(rec_packet_deriv), 0);
				HandlePacket(g, pID, &p);
				break;
			}
			case DEP_DERIV_1BUFF: {
				PacketBuffer1 p(DEP_EMPTY);
				nReadBytes = recv_s(Client, &p, GetPacketSize(rec_packet_deriv), 0);
				HandlePacket(g, pID, &p);
				break;
			}
			case DEP_DERIV_2BUFF:
				PacketBuffer2 p(DEP_EMPTY);
				nReadBytes = recv_s(Client, &p, GetPacketSize(rec_packet_deriv), 0);
				HandlePacket(g, pID, &p);
				break;
			}

			// Unblock our socket again
			iMode = 1;
			ioctlsocket(Client, FIONBIO, &iMode);
		}

		if (g->players[pID].ReadyToSendPackets) {
			g->players[pID].ReadyToSendPackets = false;
			for (int i = 0; i < MAX_PACKETS_SERVER; i++) {
				if (!g->players[pID].PacketQueue[i].empty()) {
					// Send the packet deriv ID first
					uint16_t send_packet_deriv = g->players[pID].PacketQueue[i].p()->deriv();
					int nSendBytes = send_s(Client, &send_packet_deriv, sizeof(uint16_t), 0);

					// Block the socket
					iMode = 0;
					ioctlsocket(Client, FIONBIO, &iMode);

					// Then send the actual packet
					if (send_packet_deriv != DEP_EMPTY) {
						nSendBytes = send_s(Client, g->players[pID].PacketQueue[i].p(), GetPacketSize(send_packet_deriv), 0);
					} else {
						cout << "Error: Can't send empty packet.\n";
					}

					// Unblock the socket
					iMode = 1;
					ioctlsocket(Client, FIONBIO, &iMode);

					// TODO: Fix memory leak
					g->players[pID].PacketQueue[i].free();

					if (nSendBytes == SOCKET_ERROR) {
						g->SocketUsed[pID] = false;
						cout << "Client disconnected! (" << pID << ") \n";
						shutdown(Client, SD_SEND); // Shutdown our socket		
						closesocket(Client); // Close our socket entirely
						return 0;
					}
				}
			}
		}
	}
	return 0;
}

char* GetIP(SOCKET Client) {
	SOCKADDR_IN client_info = { 0 };
	int addrsize = sizeof(client_info);
	getpeername(Client, (sockaddr*)&client_info, &addrsize);
	return inet_ntoa(client_info.sin_addr);
}

void LoadSettings() {
	cout << "Loading settings...\n";
	ifstream file("config.ini");
	file >> port;
}