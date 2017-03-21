#pragma once

class Game;
class Map;
struct SpriteStruct;
struct ALLEGRO_BITMAP;
struct ALLEGRO_COLOR;

class TMiniMap {
public:
	TMiniMap() {}
	
public:
	void Draw(Game* g, SpriteStruct* sprites, int x, int y, int w = 150, int h = 150);

private:
	void Create(Game* g, SpriteStruct* sprites);
	void DrawTile(SpriteStruct* sprites, int x, int y, int type);
	void DrawTree(SpriteStruct* sprites, int x, int y);

private:
	ALLEGRO_BITMAP* _MiniMap = nullptr;

private:
	static const int _BlockSize;
};