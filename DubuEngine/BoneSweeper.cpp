#include "Game.h"
#include "BoneSweeper.h"
#include "ScaledDraw.h"
#include "GameHandler.h"

void SpawnRandomMines(Game* g, int mines, int x, int y, int w, int h) {
	for (int i = 0; i < mines; ++i) {
		int spawn_x = x + rand() % w;
		int spawn_y = y + rand() % h;
		if (!TileIsWater(g->map.tile[spawn_x][spawn_y])) {
			g->map.zone[spawn_x][spawn_y].BoneSweeperReal = Zone::BoneSweeper::Mine;
		}
	}
}

void CalculateRealBoneSweeper(Game* g) {
	// Loop trough every square
	for (int x = 0; x < Map::MAP_SIZE_X; ++x) {
		for (int y = 0; y < Map::MAP_SIZE_Y; ++y) {

			// Make sure its not a mine and not water
			if (g->map.zone[x][y].BoneSweeperReal != Zone::BoneSweeper::Mine &&
				!TileIsWater(g->map.tile[x][y])) {

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

bool IsBoneSweeperKnownTypeThis(Game* g, Zone::BoneSweeper type, int x, int y) {
	// I use this function so I dont need to continously check whether x/y are in bound range
	if (x > 0 && y > 0 && x < Map::MAP_SIZE_X && y < Map::MAP_SIZE_Y) {
		if (g->map.zone[x][y].BoneSweeperKnown == type) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

void ClearBoneSweeper(Game* g) {
	for (int x = 0; x < Map::MAP_SIZE_X; x++) {
		for (int y = 0; y < Map::MAP_SIZE_Y; y++) {
			g->map.zone[x][y].BoneSweeperReal = Zone::BoneSweeper::None;
			g->map.zone[x][y].BoneSweeperKnown = Zone::BoneSweeper::None;
		}
	}
}

void RevealBoneSweeperSquare(Game* g, int x, int y) {
	// I use this function so I dont need to continously check whether x/y are in bound range
	if (x > 0 && y > 0 && x < Map::MAP_SIZE_X && y < Map::MAP_SIZE_Y) {
		g->map.zone[x][y].BoneSweeperKnown = g->map.zone[x][y].BoneSweeperReal;
	}
}

void DigBoneSweeper(Game* g, int tx, int ty) {
	auto dig_target = g->map.zone[tx][ty].BoneSweeperReal;
	if (dig_target == Zone::BoneSweeper::Tunnel) {
		// Store the coordinates of squares to proccess
		std::vector<pair<int, int>> targets;

		// Add this starting square to the vector
		targets.push_back(make_pair(tx, ty));

		// Reveal this part of tunnel
		RevealBoneSweeperSquare(g, tx, ty);

		// Wether the while loop should continue filling squares
		bool keep_filling = true;
		while (keep_filling) {
			std::vector<pair<int, int>> next_targets;
			for (size_t i = 0; i < targets.size(); ++i) {
				// x/y from this target
				int x = targets[i].first;
				int y = targets[i].second;

				// Up
				if (IsBoneSweeperKnownTypeThis(g, Zone::BoneSweeper::None, x, y - 1)) {
					auto working_zone = g->map.zone[x][y - 1].BoneSweeperReal;
					if (working_zone == Zone::BoneSweeper::Tunnel) {

						// Reveal this real value
						RevealBoneSweeperSquare(g, x, y - 1);

						// Add it to vector for further filling
						next_targets.push_back(make_pair(x, y - 1));
					} else if(working_zone > Zone::BoneSweeper::Tunnel) {
						// Reveal this number
						RevealBoneSweeperSquare(g, x, y - 1);
					}
				}

				// Down
				if (IsBoneSweeperKnownTypeThis(g, Zone::BoneSweeper::None, x, y + 1)) {
					auto working_zone = g->map.zone[x][y + 1].BoneSweeperReal;
					if (working_zone == Zone::BoneSweeper::Tunnel) {
						RevealBoneSweeperSquare(g, x, y + 1);
						next_targets.push_back(make_pair(x, y + 1));
					} else if (working_zone > Zone::BoneSweeper::Tunnel) {
						RevealBoneSweeperSquare(g, x, y + 1);
					}
				}

				// Left
				if (IsBoneSweeperKnownTypeThis(g, Zone::BoneSweeper::None, x - 1, y)) {
					auto working_zone = g->map.zone[x - 1][y].BoneSweeperReal;
					if (working_zone == Zone::BoneSweeper::Tunnel) {
						RevealBoneSweeperSquare(g, x - 1, y);
						next_targets.push_back(make_pair(x - 1, y));
					} else if (working_zone > Zone::BoneSweeper::Tunnel) {
						RevealBoneSweeperSquare(g, x - 1, y);
					}
				}

				// Right
				if (IsBoneSweeperKnownTypeThis(g, Zone::BoneSweeper::None, x + 1, y)) {
					auto working_zone = g->map.zone[x + 1][y].BoneSweeperReal;
					if (working_zone == Zone::BoneSweeper::Tunnel) {
						RevealBoneSweeperSquare(g, x + 1, y);
						next_targets.push_back(make_pair(x + 1, y));
					} else if (working_zone > Zone::BoneSweeper::Tunnel) {
						RevealBoneSweeperSquare(g, x + 1, y);
					}
				}
			}

			// Decide wether we continue loop
			if (next_targets.size() > 0) {
				targets = next_targets;
			} else {
				keep_filling = false;
			}
		}

	} else if(dig_target == Zone::BoneSweeper::Mine) {
		// Game over + explosion and whatever else
		ClearBoneSweeper(g);
		GameOver(g);
	} else {
		// Reveal this number
		RevealBoneSweeperSquare(g, tx, ty);
	}
}