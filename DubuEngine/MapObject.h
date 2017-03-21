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
		EMapObjectPeePuddle,
		EMapObjectPlayer_L,
		EMapObjectPlayer_M,
		EMapObjectNPC,
		EMapObjectRock_0,
		EMapObjectRock_1,
		EMapObjectRock_2,
		EMapObjectRock_3,
		EMapObjectRock_4,
		EMapObjectRock_5,
		EMapObjectRock_6,
		EMapObjectRock_7,
		EMapObjectRock_8,
		EMapObjectRock_9,
		EMapObjectCount
	};

private:
	static const EMapObjectType BushLowestID = EMapObjectBush_SG;
	static const EMapObjectType BushHighestID = EMapObjectBush_BO;
	static const EMapObjectType TreeLowestID = EMapObjectTree_BG;
	static const EMapObjectType TreeHighestID = EMapObjectTree_SO;
	static const EMapObjectType RockLowestID = EMapObjectRock_0;
	static const EMapObjectType RockHighestID = EMapObjectRock_5;
	static const EMapObjectType FlowerLowestID = EMapObjectFlower_R;
	static const EMapObjectType FlowerHighestID = EMapObjectFlower_P;
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
		bool IsBush() const;
		bool IsRock() const;			// Rock with collision
		bool IsCosmeticRock() const;	// Tiny rock with no collision
		bool IsThinRock() const;		// Thin collision rock
		bool IsWideRock() const;		// Wide collision rock
		bool IsTree() const;
		bool IsBigTree() const;
		bool IsGrass() const;
		bool IsPlayer() const;
		double DistanceToObject(const MapObject& obj);
		bool HasCollision();
		CollisionBox GetCollisionBox();
		void Draw(Game* g, SpriteStruct* sprites);
		void DrawShadow(Game* g, SpriteStruct* sprites);

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
	static MapObjectInfo::MapObject GeneratePeePuddle(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateTree(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateFlower(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateLocalPlayer(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateMultiPlayer(int x = 0, int y = 0, int id = -1);
	static MapObjectInfo::MapObject GenerateNPC(int x = 0, int y = 0, int id = -1);

private:
	static CollisionBox GenerateThinRockBox(int x, int y);
	static CollisionBox GenerateWideRockBox(int x, int y);
	static CollisionBox GenerateBigTreeBox(int x, int y);
	static CollisionBox GenerateSmallTreeBox(int x, int y);

private:
	static const MapObject _Objects[EMapObjectCount];
};