#pragma once
#include "Game.h"
void HandlePacket(Game* g, Packet* p);

/* ========================= QueueActionsPacket ===========================
 *		Creates an updated packet with local player's actions.
 *		Then adds the packet to the queue.
 */
	void QueueActionsPacket(Game* g);