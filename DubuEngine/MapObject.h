#pragma once

// Map objects
class MapObject {
public:
	MapObject() {}
	MapObject(int ID, int X, int Y) : id(ID), x(X), y(Y) {}
	int id = -1;
	int x = 0;
	int y = 0;
	int w = 64;
	int h = 64;
	double distance = 0;

	// Map object types
	/*	Key:
		S = Small
		B = Big
		G = Green
		D = Dark Green
		O = Orange
		R = Red
		C = Cyan
		P = Purple
		Player_L = Local player
		Player_M = Multi player
	*/
	enum MapObjectType {
		MapObjectBush_SG,
		MapObjectBush_BG,
		MapObjectBush_SD,
		MapObjectBush_BD,
		MapObjectBush_SO,
		MapObjectBush_BO,
		MapObjectTree_BG,
		MapObjectTree_SG,
		MapObjectTree_BD,
		MapObjectTree_SD,
		MapObjectTree_BO,
		MapObjectTree_SO,
		MapObjectFlower_R = 18,
		MapObjectFlower_C,
		MapObjectFlower_P,
		MapObjectPlayer_L = 199,
		MapObjectPlayer_M_0,
		MapObjectBone = 300
	};
};

// Return sprite ID for the given object
int GetObjectSprite(
	MapObject map_object,	// Map object to return sprite for
	int part = 0);			// Specify part of the sprite (for multi-sprite objects)