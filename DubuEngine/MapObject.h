#pragma once

#include "CollisionBox.h"

class Game;
struct SpriteStruct;

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
	enum class EMapObjectType {
		Bush_SG = 0,
		Bush_BG,
		Bush_SD,
		Bush_BD,
		Bush_SO,
		Bush_BO,
		Tree_BG,
		Tree_SG,
		Tree_BD,
		Tree_SD,
		Tree_BO,
		Tree_SO,
		Flower_R,
		Flower_C,
		Flower_P,
		Grass_0,
		Bone,
		Player_L,
		Player_M,
		EMapObjectType_Count
	};

	class MapObject {
	friend class MapObjectInfo;
	private:
		MapObject(int type, int spriteId, int numberOfSprites) :
			_SpriteId(spriteId), 
			_NumberOfSprites(numberOfSprites),
			Type(static_cast<EMapObjectType>(type)) {}

	public:
		int GetSpriteID() { return _SpriteId; }
		void ChangeSpriteID(int sprite_id) { _SpriteId = sprite_id; }

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
		double Distance = 0;
		int ID; // for multi player objects
		int AnimTimer = -1;	// -1 = Disabled, 0 = Done, 0 > Frames left

	public:
		int w = 64;
		int h = 64;
		EMapObjectType Type;
	private:
		int _SpriteId = -1;
		int _NumberOfSprites = 0;
		static const int MIN_ID = -1;
	};

public:
	static MapObjectInfo::MapObject GenerateGreenery(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateBone(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateLocalPlayer(int x = 0, int y = 0);
	static MapObjectInfo::MapObject GenerateMultiPlayer(int x = 0, int y = 0, int id = -1);

private:
	static CollisionBox GenerateBigTreeBox(int x, int y);
	static CollisionBox GenerateSmallTreeBox(int x, int y);

private:
	static const size_t _ObjectsCount = (size_t)EMapObjectType::EMapObjectType_Count;
	static const MapObject _Objects[_ObjectsCount];
};