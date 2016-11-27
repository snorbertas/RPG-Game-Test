#pragma once

/* ============================== HandleGame ==============================
 *		Handles the server-sided calculations once per tick.
 *
 *		Called from main loop.
 */
	void HandleGame(Game* g);

/* =========================== AddPacketToQueue ===========================
 *		Adds a packet into a queue for the specified player.
 *
 *		Called when desired.
 */
	int AddPacketToQueue(Player*, Packet* p);