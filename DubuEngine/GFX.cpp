#include "Game.h"
#include "ScaledDraw.h"

void DirtParticle::Draw(Game* g) {
	DrawFilledCircle(g, x + g->camera.x, y + g->camera.y, size, 120, 70, 50, opacity);
}

void HandleDirtParticles(Game* g) {
	for (size_t i = 0; i < g->gfx_dirt.size(); i++) {
		if (!g->gfx_dirt[i].Tick()) {
			g->gfx_dirt.erase(g->gfx_dirt.begin() + i);
		}
	}
}

void RenderDirtParticles(Game* g) {
	for (size_t i = 0; i < g->gfx_dirt.size(); i++) {
		g->gfx_dirt[i].Draw(g);
	}
}