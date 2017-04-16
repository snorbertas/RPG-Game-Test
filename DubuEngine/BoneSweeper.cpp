#include "Game.h"
#include "BoneSweeper.h"
#include "ScaledDraw.h"

void SpawnRandomMines(Game* g, int mines, int x, int y, int w, int h) {
	for (int i = 0; i < mines; ++i) {
		int spawn_x = x + rand() % w;
		int spawn_y = y + rand() % h;
		g->map.zone[spawn_x][spawn_y].BoneSweeperReal = Zone::BoneSweeper::Mine;
	}
}

void CalculateRealBoneSweeper(Game* g) {
	// Loop trough every square
	for (int x = 0; x < Map::MAP_SIZE_X; ++x) {
		for (int y = 0; y < Map::MAP_SIZE_Y; ++y) {

			// Make sure its not a mine
			if (g->map.zone[x][y].BoneSweeperReal != Zone::BoneSweeper::Mine) {

				// Count the mines
				int mines = 0;

				// Loop trough 9x9 grid around this square
				for (int tx = x - 1; tx < x + 2; ++tx) {
					for (int ty = y - 1; ty < y + 2; ++ty) {

						// Check if the target is legal
						if (tx > 0 && tx < Map::MAP_SIZE_X && ty > 0 && ty < Map::MAP_SIZE_Y) {
							if (g->map.zone[tx][ty].BoneSweeperReal == Zone::BoneSweeper::Mine) {

								// It's a mine!
								mines++;
							}
						}
					}
				}

				// Mark it
				g->map.zone[x][y].BoneSweeperReal = (Zone::BoneSweeper)mines;
			}
		}
	}
}

void RenderKnownBoneSweeperInfo(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font) {
	for (int x = 0; x < Map::MAP_SIZE_X; ++x) {
		for (int y = 0; y < Map::MAP_SIZE_Y; ++y) {

			// x/y
			int draw_x = x * Map::TILE_SIZE;
			int draw_y = y * Map::TILE_SIZE;

			// Make sure square is within our camera (maybe better to adjust loop size instead)
			if (draw_x + Map::TILE_SIZE > -g->camera.x && draw_x < -g->camera.x + g->BWIDTH + Map::TILE_SIZE &&
				draw_y + Map::TILE_SIZE > -g->camera.y && draw_y < -g->camera.y + g->BHEIGHT + Map::TILE_SIZE) {

				// Check this known square
				Zone::BoneSweeper known = g->map.zone[x][y].BoneSweeperKnown;
				if (known != Zone::BoneSweeper::None) {

					// Text scale ratios
					float r_x = (float)g->s_x / (float)g->BWIDTH;
					float r_y = (float)g->s_y / (float)g->BHEIGHT;

					// Render it
					if (known == Zone::BoneSweeper::Mine) {
						// Mine
						DrawImage(g, sprites->img_interface[SPRITE_FLAG_RED],
							draw_x + g->camera.x, draw_y + g->camera.y,
							0, 0.75);

					} else if (known == Zone::BoneSweeper::Maybe) {
						// Not sure
						DrawImage(g, sprites->img_interface[SPRITE_FLAG_YELLOW],
							draw_x + g->camera.x,
							draw_y + g->camera.y,
							0, 0.75);

					} else if (known == Zone::BoneSweeper::Tunnel) {
						// Tunnel (0)
						DrawText(font[2], 255, 255, 255,
							draw_x + (Map::TILE_SIZE / 2) + g->camera.x,
							draw_y + (Map::TILE_SIZE / 2) + g->camera.y - 10,
							ALLEGRO_ALIGN_CENTER, ".");

					} else {
						// Touching
						DrawText(font[2], 255, 255, 255,
							draw_x + (Map::TILE_SIZE / 2) + g->camera.x,
							draw_y + (Map::TILE_SIZE / 2) + g->camera.y - 10,
							ALLEGRO_ALIGN_CENTER, "%i", (int)known);

					}
				}
			}
		}
	}

	// Also render an outline for square on which player is standing
	int square_x = round(g->pl.x / Map::TILE_SIZE) * Map::TILE_SIZE;
	int square_y = round(g->pl.y / Map::TILE_SIZE) * Map::TILE_SIZE;
	DrawOutline(g,
		square_x + g->camera.x, square_y + g->camera.y,
		Map::TILE_SIZE, Map::TILE_SIZE,
		0, 64, 0, 2.0, 0.2);

	// Plus for mouse cursor (just an idea, more use later)
	square_x = round((-g->camera.x + g->pos_x) / Map::TILE_SIZE) * Map::TILE_SIZE;
	square_y = round((-g->camera.y + g->pos_y) / Map::TILE_SIZE) * Map::TILE_SIZE;
	DrawOutline(g,
		square_x + g->camera.x, square_y + g->camera.y,
		Map::TILE_SIZE, Map::TILE_SIZE,
		64, 0, 0, 2.0, 0.2);
}