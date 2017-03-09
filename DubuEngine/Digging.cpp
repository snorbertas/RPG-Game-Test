#include "Digging.h"
#include "ScaledDraw.h"
#include "Collision.h"
#include "GameHandler.h"
#include "PacketHandler.h"

void StartDigging(Game* g, Player* p) {
	// Local player
	if (p->pID == -1) {
		p->digging = true;
		p->dig_timer = p->dig_duration;

		// Queue packet to server
		QueueActionsPacket(g);
	}
}

void HandleDigging(Game * g, Player* p) {
	// Check if player is digging
	if (p->digging) {

		// Update timer
		--p->dig_timer;
		if (p->dig_timer <= 0) {
			p->digging = false;

			// Bone collision check for local player
			if (p->pID == -1) {
				CollisionBox col_player(
					g->pl.x + g->pl.w / 2,
					g->pl.y + g->pl.h / 2,
					1, 1);
				for (size_t i = 0; i < g->map.Bones.size(); i++) {
					CollisionBox col_bone(g->map.Bones[i].x, g->map.Bones[i].y, 64, 64);
					if (collide(col_player, col_bone)) {
						g->map.Bones[i] = g->map.Bones.back();
						g->map.Bones.pop_back();
						g->pl.bones_found++;
						break;
					}
				}
			}
		}
	}
}

void RenderDigging(Game * g, SpriteStruct* sprites) {
	// Local digging
	if (g->pl.digging) {
		int frame = g->pl.dig_timer - (g->pl.dig_timer / 8) * 8;
		frame = 3 - (frame / 2);
		DrawImage(g, sprites->img_gfx[frame],
			g->pl.x + g->camera.x,
			g->pl.y + g->camera.y, 0);
	}

	// Multi-player digging
	if (g->connected) {
		for (int i = 0; i < g->MAX_PLAYERS; i++) {
			Player* p = &g->Players[i];
			if (p->connected) {
				if (p->digging) {
					int frame = p->dig_timer - (p->dig_timer / 8) * 8;
					frame = 3 - (frame / 2);
					DrawImage(g, sprites->img_gfx[frame],
						p->x + g->camera.x,
						p->y + g->camera.y, 0);
				}
			}
		}
	}
}