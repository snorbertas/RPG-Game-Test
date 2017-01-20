#include "MapObject.h"

#include "Game.h"
#include "GameRenderer.h"
#include "ScaledDraw.h"
const MapObjectInfo::MapObject MapObjectInfo::_Objects[_ObjectsCount] = {
	/* Todo, create an enum in sprites.h to store sprite IDs for objects
	// Perhaps we don't need this once we do that ^
	// # of sprites isn't very helpful so we could take this whole thing into the draw function
	// Type, sprite, # of sprites*/
	MapObject((int)EMapObjectType::Bush_SG, ObjectSprite::Bush_SG, 1),
	MapObject((int)EMapObjectType::Bush_BG, ObjectSprite::Bush_BG, 1),
	MapObject((int)EMapObjectType::Bush_SD, ObjectSprite::Bush_SD, 1),
	MapObject((int)EMapObjectType::Bush_BD, ObjectSprite::Bush_BD, 1),
	MapObject((int)EMapObjectType::Bush_SO, ObjectSprite::Bush_SO, 1),
	MapObject((int)EMapObjectType::Bush_BO, ObjectSprite::Bush_BO, 1),
	MapObject((int)EMapObjectType::Tree_BG, ObjectSprite::Tree_BG, 2),
	MapObject((int)EMapObjectType::Tree_SG, ObjectSprite::Tree_SG, 2),
	MapObject((int)EMapObjectType::Tree_BD, ObjectSprite::Tree_BD, 2),
	MapObject((int)EMapObjectType::Tree_SD, ObjectSprite::Tree_SD, 2),
	MapObject((int)EMapObjectType::Tree_BO, ObjectSprite::Tree_BO, 2),
	MapObject((int)EMapObjectType::Tree_SO, ObjectSprite::Tree_SO, 2),
	MapObject((int)EMapObjectType::Flower_R, ObjectSprite::Flower_R, 1),
	MapObject((int)EMapObjectType::Flower_C, ObjectSprite::Flower_C, 1),
	MapObject((int)EMapObjectType::Flower_P, ObjectSprite::Flower_P, 1),
	MapObject((int)EMapObjectType::Grass_0, ObjectSprite::Grass_0, 1),
	MapObject((int)EMapObjectType::Bone, -1, 0),
	MapObject((int)EMapObjectType::Player_L, -1, 0),
	MapObject((int)EMapObjectType::Player_M, -1, 0),
};

bool MapObjectInfo::MapObject::IsBush() {
	return EMapObjectType::Bush_SG <= Type && Type <= EMapObjectType::Bush_BO;
}

bool MapObjectInfo::MapObject::IsTree() {
	return EMapObjectType::Tree_BG <= Type && Type <= EMapObjectType::Tree_SO;
}

bool MapObjectInfo::MapObject::IsBigTree() {
	return Type == EMapObjectType::Tree_BD || 
		   Type == EMapObjectType::Tree_BG || 
		   Type == EMapObjectType::Tree_BO;
}

bool MapObjectInfo::MapObject::IsGrass() {
	return EMapObjectType::Grass_0 == Type;
}

bool MapObjectInfo::MapObject::IsPlayer() {
	return Type == EMapObjectType::Player_L || Type == EMapObjectType::Player_M;
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
	if (Type == EType::Grass_0) {
		// Tall Grass
		DrawImage(g,
			img_object[_SpriteId],
			x + g->camera.x - 18,
			y + g->camera.y - 36,
			0);
	} else if (!IsPlayer()) {
		// Rendering immobile objects
		DrawImage(g,
			img_object[_SpriteId],
			x + g->camera.x,
			y + g->camera.y,
			0);

		// If it's a tree
		if (IsTree()) {
			// Draw the top of tree
			DrawImage(g,
				img_object[_SpriteId + 1],
				x + g->camera.x,
				y - h + g->camera.y,
				0);
		}
	} else if (Type == EMapObjectType::Player_L) {
		// Rendering local player
		RenderPlayer(g, g->pl, sprites);
	} else {
		// Rendering multi player
		RenderPlayer(g, g->Players[ID], sprites);
	}
}

MapObjectInfo::MapObject MapObjectInfo::GenerateGreenery(int x, int y) {
	MapObject object = _Objects[rand() % 17];
	object.x = x;
	object.y = y;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateBone(int x, int y) {
	MapObject object = _Objects[(int)EMapObjectType::Bone];
	object.x = x;
	object.y = y;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateLocalPlayer(int x, int y) {
	MapObject object = _Objects[(int)EMapObjectType::Player_L];
	object.x = x;
	object.y = y;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateMultiPlayer(int x, int y, int id) {
	MapObject object = _Objects[(int)EMapObjectType::Player_M];
	object.x = x;
	object.y = y;
	object.ID = id;
	return object;
}

CollisionBox MapObjectInfo::GenerateBigTreeBox(int x, int y) {
	return CollisionBox(x + 15, y + 54, 34, 9);
}

CollisionBox MapObjectInfo::GenerateSmallTreeBox(int x, int y) {
	return CollisionBox(x + 17, y + 56, 28, 7);
}