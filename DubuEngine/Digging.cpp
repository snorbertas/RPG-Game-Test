#include "Digging.h"
#include "ScaledDraw.h"

void StartDigging(Game* g, Player* p) {
	// Local player
	if (p->pID == -1) {
		p->digging = true;
		p->dig_timer = p->dig_duration;
	}
}

void HandleDigging(Player* p) {
	if (p->digging) {
		p->dig_timer--;
		if (p->dig_timer <= 0) {
			p->digging = false;
			// Rewards?
		}
	}
}

void RenderDigging(Game * g, SpriteStruct* sprites) {
	if (g->pl.digging) {
		int frame = g->pl.dig_timer - (g->pl.dig_timer / 8) * 8;
		frame = 3 - (frame / 2);
		DrawImage(g, sprites->img_gfx[frame],
			g->pl.x + g->camera.x,
			g->pl.y + g->camera.y, 0);
	}
}