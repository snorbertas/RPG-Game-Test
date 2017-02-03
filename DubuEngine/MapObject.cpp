#include "MapObject.h"

#include "Game.h"
#include "GameRenderer.h"
#include "ScaledDraw.h"
#include "Sprites.h"

using ESprite = EObjectSprite;

const MapObjectInfo::MapObject MapObjectInfo::_Objects[MapObjectInfo::EMapObjectCount] = {
	MapObject(0,  EObjectSpriteBush_SG), 
	MapObject(1,  EObjectSpriteBush_BG), 
	MapObject(2,  EObjectSpriteBush_SD), 
	MapObject(3,  EObjectSpriteBush_BD), 
	MapObject(4,  EObjectSpriteBush_SO), 
	MapObject(5,  EObjectSpriteBush_BO), 
	MapObject(6,  EObjectSpriteTree_BG),
	MapObject(7,  EObjectSpriteTree_SG), 
	MapObject(8,  EObjectSpriteTree_BD),
	MapObject(9,  EObjectSpriteTree_SD), 
	MapObject(10, EObjectSpriteTree_BO),
	MapObject(11, EObjectSpriteTree_SO),
	MapObject(12, EObjectSpriteFlower_R),
	MapObject(13, EObjectSpriteFlower_C), 
	MapObject(14, EObjectSpriteFlower_P), 
	MapObject(15, EObjectSpriteGrass_0), 
	MapObject(16, EObjectSpriteUndefined), // bone without sprite 
	MapObject(17, EObjectSpriteUndefined), // local player
	MapObject(18, EObjectSpriteUndefined), // multi player
};

bool MapObjectInfo::MapObject::IsBush() {
	return BushLowestID <= Type && Type <= BushHighestID;
}

bool MapObjectInfo::MapObject::IsTree() {
	return TreeLowestID <= Type && Type <= TreeHighestID;
}

bool MapObjectInfo::MapObject::IsBigTree() {
	return Type == EMapObjectTree_BD || 
		   Type == EMapObjectTree_BG || 
		   Type == EMapObjectTree_BO;
}

bool MapObjectInfo::MapObject::IsGrass() {
	return Type == EMapObjectGrass;
}

bool MapObjectInfo::MapObject::IsPlayer() {
	return Type == EMapObjectPlayer_L || Type == EMapObjectPlayer_M;
}

bool MapObjectInfo::MapObject::HasCollision() {
	return IsTree();
}

CollisionBox MapObjectInfo::MapObject::GetCollisionBox() {
	if (!HasCollision())
		return CollisionBox(x, y, 0, 0);
	if (IsBigTree())
		return GenerateBigTreeBox(x, y);
	else
		return GenerateSmallTreeBox(x, y);
}

void MapObjectInfo::MapObject::Draw(Game* g, SpriteStruct* sprites) {
	using EType = EMapObjectType;
	auto& img_object = sprites->img_object;
	if (IsGrass()) {
		// Tall Grass
		DrawImage(g,
			img_object[SpriteId],
			x + g->camera.x - 18,
			y + g->camera.y - 36,
			0);
	} else if (!IsPlayer()) {
		// Rendering immobile objects
		DrawImage(g,
			img_object[SpriteId],
			x + g->camera.x,
			y + g->camera.y,
			0);

		// If it's a tree
		if (IsTree()) {
			// Draw the top of tree
			DrawImage(g,
				img_object[SpriteId + 1],
				x + g->camera.x,
				y - h + g->camera.y,
				0);
		}
	} else if (Type == EMapObjectPlayer_L) {
		// Rendering local player
		RenderPlayer(g, g->pl, sprites);
	} else {
		// Rendering multi player
		RenderPlayer(g, g->Players[ID], sprites);
	}
}

MapObjectInfo::MapObject MapObjectInfo::GenerateObjectByType(EMapObjectType type, int x, int y, int id) {
	MapObject object = _Objects[type];
	object.x = x;
	object.y = y;
	object.ID = id;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateGreenery(int x, int y) {
	int index = GreeneryLowestID + rand() % (GreeneryHighestID - GreeneryLowestID + 1);
	return GenerateObjectByType(static_cast<EMapObjectType>(index), x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateBone(int x, int y) {
	return GenerateObjectByType(EMapObjectBone, x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateTree(int x, int y) {
	int index = TreeLowestID + (rand() % (TreeHighestID - TreeLowestID + 1));
	return GenerateObjectByType(static_cast<EMapObjectType>(index), x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateLocalPlayer(int x, int y) {
	return GenerateObjectByType(EMapObjectPlayer_L, x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateMultiPlayer(int x, int y, int id) {
	return GenerateObjectByType(EMapObjectPlayer_M, x, y, id);
}

CollisionBox MapObjectInfo::GenerateBigTreeBox(int x, int y) {
	return CollisionBox(x + 15, y + 54, 34, 9);
}

CollisionBox MapObjectInfo::GenerateSmallTreeBox(int x, int y) {
	return CollisionBox(x + 17, y + 56, 28, 7);
}