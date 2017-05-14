#include <windows.h>
#include "Game.h"
#include "GameHandler.h"

void LoadMultiplayer(Game* g) {
	// Spawn
	g->game_mode = GameMode::GM_BoneHunt;
	g->pl.x = 3200;
	g->pl.y = 3200;

	// Start loading screen
	HideAllInterfaces(g, INTERFACE_GENERATING);
	g->Interfaces[INTERFACE_GENERATING].Show();
	srand(al_get_time() * 1000);
	g->menu.tip_id = rand() % 2;

	// Record time
	double current_time = al_get_time();

	// Generate map
	g->map.ChangeForestMode(1);
	g->map.Generate(1);

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
	HideAllInterfaces(g, INTERFACE_CHAT);
	g->Interfaces[INTERFACE_VERSION].Show();
	g->Interfaces[INTERFACE_CHAT].Show();
	g->Interfaces[INTERFACE_RADAR].Show();
	g->Interfaces[INTERFACE_STATS].Show();
	g->Interfaces[INTERFACE_SCORE].Show();
	g->Interfaces[INTERFACE_MINI_MAP].Show();

	// Welcome interface
	g->welcome_interface.ResetInterface(g);
	g->welcome_interface.SetDimensions((g->BWIDTH / 2) - 400, (g->BHEIGHT / 2) - 250, 800, 500);
	if (g->game_mode == GameMode::GM_BoneHunt) {
		g->welcome_interface.LoadTextFromFile("dec/BoneHunt.dec");
	} else if(g->game_mode == GameMode::GM_BoneSweeper) {
		g->welcome_interface.LoadTextFromFile("dec/BoneSweeper.dec");
	}
	g->welcome_interface.InterpretAllRawText();
	g->Interfaces[INTERFACE_WELCOME].Show();


	// Reset, fade and done :)
	g->menu.done_loading = false;
	Fade(g, 1);
}