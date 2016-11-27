#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
class Game;

#define MAX_EVENT_FUNCTIONS 20

// Function pointer definitions
typedef void(*TimerEventFunction)(Game*, ALLEGRO_SAMPLE**);
typedef void(*MouseEventFunction)(Game*, int, bool, ALLEGRO_SAMPLE**);
typedef void(*KeyboardEventFunction)(Game*, int, bool, ALLEGRO_SAMPLE**);