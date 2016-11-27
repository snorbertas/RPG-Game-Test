#pragma once

/* ============================== HandleGame ==============================
 *		Handles a menu-tick.
 *
 *		Called from the main function.
 */
	void HandleMenu(Game* g,			// Game pointer
		ALLEGRO_SAMPLE** sample_sfx);	// Pointer to array of sounds


/* ============================== HandleIntro ==============================
 *		Handles menu intro when application opens.
 *
 *		Called from the HandleGame function.
 */
	void HandleIntro(Game* g);

/* ========================== HandleMenuBackground =========================
 *		Handles menu background animation.
 *
 *		Called from the HandleGame function.
 */
	void HandleMenuBackground(Game* g);