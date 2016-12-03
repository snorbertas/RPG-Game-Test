#pragma once
#include <allegro5/allegro_font.h>

/* ============================== RenderGame ==============================
 *		Main render function for gameplay. Renders a whole frame for the game.
 *		This function's job is to call all functions related to game-rendering
 *		to render each layer/step in correct order.
 *
 *		Called once per tick from the main() function.
 */
	void RenderGame(Game* g,				// Game pointer
		SpriteStruct* sprites,				// Sprites
		ALLEGRO_FONT** font);				// Pointer to array of fonts


/* ============================= RenderPlayer =============================
 *		Renders the player.
 *
 *		Called once per tick from the RenderGame() function.
 */
	void RenderPlayer(Game* g, Player& pl, SpriteStruct* sprites);