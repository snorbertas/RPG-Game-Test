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
 *		Renders a player.
 *
 *		Called when desired.
 */
	void RenderPlayer(Game* g, Player& pl, SpriteStruct* sprites);


/* ========================== RenderPlayerShadow ==========================
 *		Renders a player's shadow.
 *
 *		Called when desired.
 */
	void RenderPlayerShadow(Game* g, Player& pl, SpriteStruct* sprites);


/* =============================== RenderNPC ==============================
 *		Renders an NPC.
 *
 *		Called when desired.
 */
	void RenderNPC(Game* g, NPC& npc, SpriteStruct* sprites);


/* ============================ RenderNPCShadow ===========================
 *		Renders an NPC's shadow.
 *
 *		Called when desired.
 */
	void RenderNPCShadow(Game* g, NPC& npc, SpriteStruct* sprites);


/* ======================= RenderParallaxBackground =======================
 *		Renders the background.
 *
 *		Called once per tick from the RenderGame() function.
 */
	void RenderParallaxBackground(Game* g, SpriteStruct* sprites);