#pragma once

class Game;
class Player;
struct SpriteStruct;

void StartDrinking(Game* g, Player* p);
void HandleDrinking(Game* g, Player* p);
void RenderDrinking(Game * g, SpriteStruct* sprites);