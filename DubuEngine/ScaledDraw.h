#pragma once
/*	This is basically used to draw with Allegro library but to
	match the scale with the game's settings. We don't want a static
	resolution, thus drawing a sprite with its true size pixel to pixel
	is undesirable. */

/* ============================== DrawImage ===============================
 *		Draws an image.
 *		The image will be rescaled according to the ratio between game size
 *		and the resolution setting. (g->BWIDTH, g->BHEIGHT & g->s_x, g->s_y)
 *
 *		Called from render functions.
 */
	void DrawImage(Game* g,				// Game pointer
		ALLEGRO_BITMAP* img,			// Image Pointer
		int x, int y,					// x, y coordinates
		int flags,						// 1 = flip horizontally
		float opacity=1);				// Transperency 0 to 1 (0 = invisible)


/* =========================== DrawScaledImage ============================
 *		Draws an image, scaled to desired width/height.
 *		The image will be rescaled according to the ratio between game size
 *		and the resolution setting. (g->BWIDTH, g->BHEIGHT & g->s_x, g->s_y)
 *
 *		Called from render functions.
 */
	void DrawScaledImage(Game* g,		// Game pointer
		ALLEGRO_BITMAP* img,			// Image Pointer
		int x, int y,					// x, y coordinates
		int wi, int hi,					// Width and height
		int flags,						// 1 = flip horizontally
		float opacity=1);				// Transperency 0 to 1 (0 = invisible)


/* =========================== DrawRotatedImage ===========================
 *		Draws an image, rotated by desired degrees.
 *		The image will be rescaled according to the ratio between game size
 *		and the resolution setting. (g->BWIDTH, g->BHEIGHT & g->s_x, g->s_y)
 *
 *		Called from render functions.
 */
	void DrawRotatedImage(Game* g,		// Game pointer
		ALLEGRO_BITMAP* img,			// Image Pointer
		int x, int y,					// x, y coordinates
		int degree,						// Degrees to rotate
		int flags);						// 1 = flip horizontally


/* ============================ DrawRectangle =============================
 *		Draws a rectangle.
 *		The shape will be rescaled according to the ratio between game size
 *		and the resolution setting. (g->BWIDTH, g->BHEIGHT & g->s_x, g->s_y)
 *
 *		Called from render functions.
 */
	void DrawRectangle(Game* g,			// Game pointer
		int x, int y,					// x, y coordinates
		int w, int h,					// Width and height
		int re, int gr, int bl,			// Red, green, blue colours
		float a=1);						// Transperency 0 to 1 (0 = invisible)


/* ============================= DrawOutline ==============================
 *		Draws an outline shaped as a rectangle.
 *		The shape will be rescaled according to the ratio between game size
 *		and the resolution setting. (g->BWIDTH, g->BHEIGHT & g->s_x, g->s_y)
 *
 *		Called from render functions.
 */
	void DrawOutline(Game* g,			// Game pointer
		int x, int y,					// x, y coordinates
		int w, int h,					// Width and height
		int re, int gr, int bl,			// Red, green, blue colours
		float t,						// Thickness
		int a=255);						// Transperency 0 to 255 (0 = invisible)


/* ============================== DrawCircle ==============================
 *		Draws an outline shaped as a circle.
 *		The shape will be rescaled according to the ratio between game size
 *		and the resolution setting. (g->BWIDTH, g->BHEIGHT & g->s_x, g->s_y)
 *
 *		Called from render functions.
 */
	void DrawCircle(Game* g,			// Game pointer
		int x, int y,					// x, y coordinates
		float r,						// Radius
		int s,							// Size
		int re, int gr, int bl);		// Red, green, blue colours

/* =============================== DrawText ===============================
 *		Draws text.
 *		Shortened macro, requires float r_x/r_y to be defined prior as the scale ratio.
 *
 *		Called from render functions.
 */
	#define DrawText(f, re, gr, bl, x, y, a, text, ...) al_draw_textf(f, al_map_rgb(re, gr, bl), (int)(r_x*(x)), (int)(r_y*(y)), a, text, __VA_ARGS__)
	#define DrawTextA(f, re, gr, bl, x, y, a, o, text, ...) al_draw_textf(f,  al_map_rgba_f(re, gr, bl, o), (int)(r_x*(x)), (int)(r_y*(y)), a, text, __VA_ARGS__)
	/*
	 * f = font
	 * re = red
	 * gr = green
	 * bl = blue
	 * x = x coordinate
	 * y = y coordinate
	 * a = align rules
	 * text = string of text
	 * ARGS = variables included in text */