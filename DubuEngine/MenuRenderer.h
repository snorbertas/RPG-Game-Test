#pragma once

/* ============================== RenderMenu ==============================
 *		Main render function for the menu.
 *
 *		Called once per tick from the main() function.
 */
	void RenderMenu(Game* g,				//	Game pointer
		SpriteStruct* sprites,			//	Sprites
		ALLEGRO_FONT** font);				//	Pointer to array of fonts


/* ============================ RenderLoading =============================
 *		Renders the loading screen.
 *
 *		Called once per tick from the main() function.
 */
	void RenderLoading(Game* g,				// Game pointer
		ALLEGRO_BITMAP** img_background,	// Background sprites
		ALLEGRO_BITMAP** img_sprite,		// Other sprites
		ALLEGRO_FONT** font);				// Pointer to array to fonts