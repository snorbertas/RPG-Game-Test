#include "MapObject.h"

#include "Game.h"
#include "GameRenderer.h"
#include "ScaledDraw.h"

const MapObjectInfo::MapObject MapObjectInfo::_Objects[_ObjectsCount] = {
	MapObject(0,  1, 0),
	MapObject(1,  1, 1),
	MapObject(2,  1, 2),
	MapObject(3,  1, 3),
	MapObject(4,  1, 4),
	MapObject(5,  1, 5),
	MapObject(6,  2, 6),
	MapObject(8,  2, 7),
	MapObject(10, 2, 8),
	MapObject(12, 2, 9),
	MapObject(14, 2, 10),
	MapObject(16, 2, 11),
	MapObject(18, 1, 12),
	MapObject(19, 1, 13),
	MapObject(20, 1, 14),
	MapObject(21, 1, 15),
	MapObject(22, 1, 16),
	MapObject(-1, 1, 17), // bone without sprite
	MapObject(-1, 1, 18), // local player
	MapObject(-1, 1, 19), // multi player
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
	return EMapObjectType::Grass_0 <= Type && Type <= EMapObjectType::Grass_1;
}

bool MapObjectInfo::MapObject::IsPlayer() {
	return Type == EMapObjectType::Player_L || Type == EMapObjectType::Player_M;
}

bool MapObjectInfo::MapObject::HasCollision() {
	return !IsTree();
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
	if (Type == EType::Grass_0 || Type == EType::Grass_1) {
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
		RenderPlayer(g, g->Players[ID - MIN_ID], sprites);
	}
}

MapObjectInfo::MapObject MapObjectInfo::GenerateGreenery(int x, int y) {
	MapObject object = _Objects[rand() % 17];
	object.x = x;
	object.y = y;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateBone(int x, int y) {
	MapObject object = _Objects[17];
	object.x = x;
	object.y = y;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateLocalPlayer(int x, int y) {
	MapObject object = _Objects[18];
	object.x = x;
	object.y = y;
	return object;
}

MapObjectInfo::MapObject MapObjectInfo::GenerateMultiPlayer(int x, int y, int id) {
	MapObject object = _Objects[19];
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