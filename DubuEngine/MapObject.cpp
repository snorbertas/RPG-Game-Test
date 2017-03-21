#include "MapObject.h"

#include "Game.h"
#include "GameRenderer.h"
#include "ScaledDraw.h"
#include "Sprites.h"

using ESprite = EObjectSprite;

const MapObjectInfo::MapObject MapObjectInfo::_Objects[MapObjectInfo::EMapObjectCount] = {
	MapObject(EMapObjectBush_SG,  EObjectSpriteBush_SG),
	MapObject(EMapObjectBush_BG,  EObjectSpriteBush_BG),
	MapObject(EMapObjectBush_SD,  EObjectSpriteBush_SD),
	MapObject(EMapObjectBush_BD,  EObjectSpriteBush_BD),
	MapObject(EMapObjectBush_SO,  EObjectSpriteBush_SO),
	MapObject(EMapObjectBush_BO,  EObjectSpriteBush_BO),
	MapObject(EMapObjectTree_BG,  EObjectSpriteTree_BG),
	MapObject(EMapObjectTree_SG,  EObjectSpriteTree_SG),
	MapObject(EMapObjectTree_BD,  EObjectSpriteTree_BD),
	MapObject(EMapObjectTree_SD,  EObjectSpriteTree_SD),
	MapObject(EMapObjectTree_BO, EObjectSpriteTree_BO),
	MapObject(EMapObjectTree_SO, EObjectSpriteTree_SO),
	MapObject(EMapObjectFlower_R, EObjectSpriteFlower_R),
	MapObject(EMapObjectFlower_C, EObjectSpriteFlower_C),
	MapObject(EMapObjectFlower_P, EObjectSpriteFlower_P),
	MapObject(EMapObjectGrass, EObjectSpriteGrass_0),
	MapObject(EMapObjectBone, EObjectSpriteUndefined),
	MapObject(EMapObjectPeePuddle, EObjectSpriteUndefined),
	MapObject(EMapObjectPlayer_L, EObjectSpriteUndefined),
	MapObject(EMapObjectPlayer_M, EObjectSpriteUndefined),
	MapObject(EMapObjectNPC, EObjectSpriteUndefined),
	MapObject(EMapObjectRock_0, EObjectSpriteRock_0),
	MapObject(EMapObjectRock_1, EObjectSpriteRock_1),
	MapObject(EMapObjectRock_2, EObjectSpriteRock_2),
	MapObject(EMapObjectRock_3, EObjectSpriteRock_3),
	MapObject(EMapObjectRock_4, EObjectSpriteRock_4),
	MapObject(EMapObjectRock_5, EObjectSpriteRock_5),
	MapObject(EMapObjectRock_6, EObjectSpriteRock_6),
	MapObject(EMapObjectRock_7, EObjectSpriteRock_7),
	MapObject(EMapObjectRock_8, EObjectSpriteRock_8),
	MapObject(EMapObjectRock_9, EObjectSpriteRock_9),
};

bool MapObjectInfo::MapObject::IsBush() const {
	return BushLowestID <= Type && Type <= BushHighestID;
}

bool MapObjectInfo::MapObject::IsRock() const {
	return RockLowestID <= Type && Type <= RockHighestID;
}

bool MapObjectInfo::MapObject::IsCosmeticRock() const {
	return EMapObjectRock_6 <= Type && Type <= EMapObjectRock_9;
}

bool MapObjectInfo::MapObject::IsThinRock() const {
	return RockLowestID <= Type && Type <= EMapObjectRock_3;
}

bool MapObjectInfo::MapObject::IsWideRock() const {
	return EMapObjectRock_4 <= Type && Type <= RockHighestID;
}

bool MapObjectInfo::MapObject::IsTree() const {
	return TreeLowestID <= Type && Type <= TreeHighestID;
}

bool MapObjectInfo::MapObject::IsBigTree() const {
	return Type == EMapObjectTree_BD || 
		   Type == EMapObjectTree_BG || 
		   Type == EMapObjectTree_BO;
}

bool MapObjectInfo::MapObject::IsGrass() const {
	return Type == EMapObjectGrass;
}

bool MapObjectInfo::MapObject::IsPlayer() const {
	return Type == EMapObjectPlayer_L || Type == EMapObjectPlayer_M || Type == EMapObjectNPC;
}

double MapObjectInfo::MapObject::DistanceToObject(const MapObjectInfo::MapObject& obj) {
	return sqrt(double((x - obj.x) * (x - obj.x) + (y - obj.y) * (y - obj.y)));
}

bool MapObjectInfo::MapObject::HasCollision() {
	return IsTree() || IsRock();
}

CollisionBox MapObjectInfo::MapObject::GetCollisionBox() {
	if (!HasCollision())
		return CollisionBox(x, y, 0, 0);
	if (IsThinRock())
		return GenerateThinRockBox(x, y);
	else if (IsWideRock())
		return GenerateWideRockBox(x, y);
	else if (IsBigTree())
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
	} else if (Type == EMapObjectNPC) {
		// Rendering an NPC
		RenderNPC(g, g->npc[ID], sprites);
	} else if (Type == EMapObjectPlayer_L) {
		// Rendering local player
		RenderPlayer(g, g->pl, sprites);
	} else {
		// Rendering multi player
		RenderPlayer(g, g->Players[ID], sprites);
	}
}

void MapObjectInfo::MapObject::DrawShadow(Game* g, SpriteStruct* sprites) {
	if(g->weather.cycle.sunlight.active){
		int angle = g->weather.cycle.sunlight.position;
		float opacity = g->weather.cycle.sunlight.brightness;

		using EType = EMapObjectType;
		auto& img_gfx = sprites->img_gfx;
		auto& img_object = sprites->img_object;
		if (IsGrass()) {
			// Tall Grass
			angle = 0;
			DrawRotatedShadow(g,
				img_object[SpriteId],
				x + 32 + g->camera.x,
				y + 60 + g->camera.y - 7,
				angle, ALLEGRO_FLIP_VERTICAL, opacity);
		} else if (IsRock() || IsCosmeticRock()){
			// Rocks
			angle = 0;
			DrawRotatedShadow(g,
				img_object[SpriteId],
				x + 36 + g->camera.x,
				y + 63 + g->camera.y,
				angle, ALLEGRO_FLIP_VERTICAL, opacity);
		} else if (!IsPlayer()) {
			// Rendering immobile objects
			if (!IsTree()) {
				DrawRotatedShadow(g,
					img_object[SpriteId],
					x + 32 + g->camera.x,
					y + 60 + g->camera.y,
					angle, ALLEGRO_FLIP_VERTICAL, opacity);
			} else {
				int gfx_id = SPRITE_SHADOW_SMALL_TREE;
				if (IsBigTree()) gfx_id = SPRITE_SHADOW_BIG_TREE;
				DrawRotatedShadow(g,
					img_gfx[gfx_id],
					x + 32 + g->camera.x,
					y + 60 + g->camera.y,
					angle, ALLEGRO_FLIP_VERTICAL, opacity);
			}
		} else if (Type == EMapObjectNPC) {
			// Rendering local player
			RenderNPCShadow(g, g->npc[ID], sprites);
		} else if (Type == EMapObjectPlayer_L) {
			// Rendering local player
			RenderPlayerShadow(g, g->pl, sprites);
		} else {
			// Rendering multi player
			RenderPlayerShadow(g, g->Players[ID], sprites);
		}
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

MapObjectInfo::MapObject MapObjectInfo::GeneratePeePuddle(int x, int y) {
	return GenerateObjectByType(EMapObjectPeePuddle, x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateTree(int x, int y) {
	int index = TreeLowestID + (rand() % (TreeHighestID - TreeLowestID + 1));
	return GenerateObjectByType(static_cast<EMapObjectType>(index), x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateFlower(int x, int y) {
	int index = FlowerLowestID + (rand() % (FlowerHighestID - FlowerLowestID + 1));
	return GenerateObjectByType(static_cast<EMapObjectType>(index), x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateLocalPlayer(int x, int y) {
	return GenerateObjectByType(EMapObjectPlayer_L, x, y);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateNPC(int x, int y, int id) {
	return GenerateObjectByType(EMapObjectNPC, x, y, id);
}

MapObjectInfo::MapObject MapObjectInfo::GenerateMultiPlayer(int x, int y, int id) {
	return GenerateObjectByType(EMapObjectPlayer_M, x, y, id);
}

CollisionBox MapObjectInfo::GenerateThinRockBox(int x, int y) {
	return CollisionBox(x, y + 54, 70, 9);
}

CollisionBox MapObjectInfo::GenerateWideRockBox(int x, int y) {
	return CollisionBox(x, y + 54, 85, 9);
}

CollisionBox MapObjectInfo::GenerateBigTreeBox(int x, int y) {
	return CollisionBox(x + 15, y + 54, 34, 9);
}

CollisionBox MapObjectInfo::GenerateSmallTreeBox(int x, int y) {
	return CollisionBox(x + 17, y + 56, 28, 7);
}