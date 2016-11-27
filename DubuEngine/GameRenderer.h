#pragma once

/* ============================== RenderGame ==============================
 *		Main render function for gameplay. Renders a whole frame for the game.
 *		This function's job is to call all functions related to game-rendering
 *		to render each layer/step in correct order.
 *
 *		Called once per tick from the main() function.
 */
	void RenderGame(Game* g,				// Game pointer
		SpriteStruct* csprites,				// Sprites
		ALLEGRO_FONT** font);				// Pointer to array of fonts