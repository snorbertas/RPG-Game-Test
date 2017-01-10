#pragma once
#include "DEEvents.h"
#include <allegro5/allegro_font.h>

class Radar
{
public:
	Radar() {};
	enum Mode {DISTANCE_SNIFF, DIRECTION_SNIFF};
	Mode mode = DIRECTION_SNIFF;
	float opacity = 0.5;
	float scale = 0.0;
	double range = 7;
	int timer = 0;
	int angle = 0;
	bool bone_detected = false;
	double closest_distance = 0.0;
};

void UpdateRadar(Game* g, ALLEGRO_SAMPLE** sample_sfx);
void RenderRadar(Game* g, ALLEGRO_FONT** font, SpriteStruct* sprites);

int RegisterRadarEvents(TimerEventFunction* t, MouseEventFunction* c, KeyboardEventFunction* p);
