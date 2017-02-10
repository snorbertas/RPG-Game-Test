#include "Peeing.h"

#include "Game.h"

void StartPeeing(Game* g, Player* p) {
	// Local player
	if (p->pID == -1) {
		p->peeing = true;
		p->pee_timer = p->pee_duration;
		--p->pee_ammo;
	}
}

void HandlePeeing(Game* g, Player* p) {
	if (!p->peeing)
		return;

	--p->pee_timer;
	if (p->pee_timer <= 0) {
		p->peeing = false;
		if (p->pID == -1) {
			g->map.AddPeePuddle(p->x, p->y);
			AddChatMessage(g->chat, "__SYSTEM__", 139, 49, 121, "Added Puddle");
		}
	}
}

void RenderPeeing(Game * g, SpriteStruct* sprites) {
}