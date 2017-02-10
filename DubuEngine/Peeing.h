#pragma once

class Game;
class Player;
struct SpriteStruct;

void StartPeeing(Game* g, Player* p);
void HandlePeeing(Game* g, Player* p);
void RenderPeeing(Game * g, SpriteStruct* sprites);