#pragma once

#include "CollisionBox.h"

class Game;
struct SpriteStruct;
enum EObjectSprite;

class MapObjectInfo {
private:
	MapObjectInfo() {}

public:
	//	Map object types
	//	Key:
	//	S = Small
	//	B = Big
	//	G = Green
	//	D = Dark Green
	//	O = Orange
	//	R = Red
	//	C = Cyan
	//	P = Purple
	//	Player_L = Local player
	//	Player_M = Multi player
	enum EMapObjectType {
		EMapObjectBush_SG = 0,
		EMapObjectBush_BG,
		EMapObjectBush_SD,
		EMapObjectBush_BD,
		EMapObjectBush_SO,
		EMapObjectBush_BO,
		EMapObjectTree_BG,
		EMapObjectTree_SG,
		EMapObjectTree_BD,
		EMapObjectTree_SD,
		EMapObjectTree_BO,
		EMapObjectTree_SO,
		EMapObjectFlower_R,
		EMapObjectFlower_C,
		EMapObjectFlower_P,
		EMapObjectGrass,
		EMapObjectBone,
		EMapObjectPlayer_L,
		EMapObjectPlayer_M,
		EMapObjectCount
	};

private:
	static const EMapObjectType BushLowestID = EMapObjectBush_SG;
	static const EMapObjectType BushHighestID = EMapObjectBush_BO;
	static const EMapObjectType TreeLowestID = EMapObjectTree_BG;
	static const EMapObjectType TreeHighestID = EMapObjectTree_SO;
	static const EMapObjectType GreeneryLowestID = EMapObjectBush_SG;
	static const EMapObjectType GreeneryHighestID = EMapObjectFlower_P;

public:
	class MapObject {
	friend class MapObjectInfo;
	private:
		MapObject(int type, EObjectSprite spriteId) :
			Type(static_cast<EMapObjectType>(type)), 
			SpriteId(spriteId) {}

	public:
		bool IsBush();
		bool IsTree();
		bool IsBigTree();
		bool IsGrass();
		bool IsPlayer();
		bool HasCollision();
		CollisionBox GetCollisionBox();
		void Draw(Game* g, SpriteStruct* sprites);

	public:
		int x = 0;
		int y = 0;
		int w = 64; // sprite width
		int h = 64; // sprite width
		double Distance = 0;
		int ID; // for multi player objects
		int AnimTimer = -1;	// -1 = Disabled, 0 = Done, 0 > Frames left
		EObjectSprite SpriteId;
		EMapObjectType Type;
	};

public:
	static MapObjectInfo::MapObject GenerateObjectByType(EMapObjectType type, int x = 0, int y = 0, int id = -1);
	static MapObjectInfo::MapObject GenerateGreenery(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateBone(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateTree(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateLocalPlayer(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateMultiPlayer(int x = 0, int y = 0, int id = -1);

private:
	static CollisionBox GenerateBigTreeBox(int x, int y);
	static CollisionBox GenerateSmallTreeBox(int x, int y);

private:
	static const MapObject _Objects[EMapObjectCount];
};