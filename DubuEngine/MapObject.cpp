#include "MapObject.h"

int GetObjectSprite(MapObject map_object, int part) {
	auto id =  map_object.id;
	if (part > 0) {
		return (id + 6);
	} else {
		return id;
	}
}