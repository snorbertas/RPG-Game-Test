#pragma once

// GameDuration structure (TODO: will improve this once figured out how timers will be used across all game modes)
class GameDuration {
public:
	bool ticking = false;					// Wether the timer is active or not
	int ticks = 0;							// Game duration in ticks
	int minutes = 0;						// Minutes since game has started
	int seconds = 0;						// Seconds (up to 59) since game has started
	bool inverted = false;					// If true, we're counting down
	int seconds_start = 120;				// Start counting down from here if inverted (in seconds)
};