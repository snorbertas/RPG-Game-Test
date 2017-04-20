#include "Peeing.h"
#include "Game.h"
#include "ScaledDraw.h"
#include "PacketHandler.h"

void StartPeeing(Game* g, Player* p) {
	// Local player
	if (p->pID == -1) {
		if (g->game_mode == GameMode::GM_BoneHunt) {
			p->peeing = true;
			p->pee_timer = p->pee_duration;
			--p->pee_ammo;

			// Queue packet to server
			QueueActionsPacket(g);
		} else if(g->game_mode == GameMode::GM_BoneSweeper){
			int x = round(p->x / Map::TILE_SIZE);
			int y = round(p->y / Map::TILE_SIZE);

			if (g->map.zone[x][y].BoneSweeperKnown == Zone::BoneSweeper::None) {
				// Mark as red flag
				g->map.zone[x][y].BoneSweeperKnown = Zone::BoneSweeper::Mine;

			} else if (g->map.zone[x][y].BoneSweeperKnown == Zone::BoneSweeper::Mine) {
				// Mark as yellow flag
				g->map.zone[x][y].BoneSweeperKnown = Zone::BoneSweeper::Maybe;

			} else if (g->map.zone[x][y].BoneSweeperKnown == Zone::BoneSweeper::Maybe) {
				// Delete the flag
				g->map.zone[x][y].BoneSweeperKnown = Zone::BoneSweeper::None;
			}
		}
	}
}

void HandlePeeing(Game* g, Player* p) {
	if (!p->peeing)
		return;

	--p->pee_timer;
	if (p->pee_timer <= 0) {
		p->peeing = false;
		if (p->pID == -1) {
			g->map.AddPeePuddle(p->x, p->y);
			g->map.MarkTerritory(round(p->x/Map::TILE_SIZE), round(p->y/Map::TILE_SIZE), -1, 3);
			AddChatMessage(g->chat, "__SYSTEM__", 139, 49, 121, "Added Puddle");
		}
	}
}

void RenderPeeing(Game * g, SpriteStruct* sprites) {
	// Flag for rendering sprite's orientation
	int flags = 0;

	// Local peeing
	if (g->pl.peeing) {
		// Decide orientation
		if (g->pl.facing == Player::Facing::FacingRight) flags = ALLEGRO_FLIP_HORIZONTAL;

		// Calculate frame
		int frame = g->pl.pee_timer - (g->pl.pee_timer / 8) * 8;
		frame = 3 - (frame / 2);

		// Render sprite
		DrawImage(g, sprites->img_gfx[frame + 4],
			g->pl.x + g->camera.x,
			g->pl.y + g->camera.y, flags);
	}

	// Multi-player peeing
	if (g->connected) {
		for (int i = 0; i < g->MAX_PLAYERS; i++) {
			Player* p = &g->Players[i];
			if (p->connected) {
				if (p->peeing) {
					// Decide orientation
					if (p->facing == Player::Facing::FacingRight) flags = ALLEGRO_FLIP_HORIZONTAL;

					// Calculate frame
					int frame = p->pee_timer - (p->pee_timer / 8) * 8;
					frame = 3 - (frame / 2);

					// Render sprite
					DrawImage(g, sprites->img_gfx[frame + 4],
						p->x + g->camera.x,
						p->y + g->camera.y, flags);
				}
			}
		}
	}
}