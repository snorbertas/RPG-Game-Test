#include "Sniffing.h"
#include "Game.h"

void CalculateBoneDistances(Game* g) {
	for (int i = 0; i < g->map.bone.size(); i++) {
		int bone_x = g->map.bone[i].x + (Map::TILE_SIZE / 2);
		int bone_y = g->map.bone[i].y + (Map::TILE_SIZE / 2);
		double player_x = g->pl.x + (Map::TILE_SIZE / 2);
		double player_y = g->pl.y + (Map::TILE_SIZE / 2);
		double distance_x = (bone_x - player_x) / Map::TILE_SIZE;
		double distance_y = (bone_y - player_y) / Map::TILE_SIZE;
		if (distance_x < 0) distance_x *= -1;
		if (distance_y < 0) distance_y *= -1;
		double distance = sqrt(pow(distance_x, 2) + pow(distance_y, 2));


		g->map.bone[i].distance = distance;
		//sqrt(d^2)
	}
}

void Sniff(Game* g) {

}