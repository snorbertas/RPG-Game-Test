#include "Digging.h"
#include "ScaledDraw.h"
#include "Collision.h"

void StartDigging(Game* g, Player* p) {
	// Local player
	if (p->pID == -1) {
		p->digging = true;
		p->dig_timer = p->dig_duration;
	}
}

void HandleDigging(Game * g, Player* p) {
	if (p->digging) {
		p->dig_timer--;
		if (p->dig_timer <= 0) {
			p->digging = false;
			if (p->pID == -1) {
				for (int i = 0; i < g->map.bone.size(); i++) {
					CollisionBox col_player(
						g->pl.x + g->pl.w / 2,
						g->pl.y + g->pl.h / 2,
						1, 1);
					CollisionBox col_bone(g->map.bone[i].x, g->map.bone[i].y, 64, 64);
					if (collide(col_player, col_bone)) {
						g->map.bone[i] = g->map.bone.back();
						g->map.bone.pop_back();
						break;
					}
				}
			}
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