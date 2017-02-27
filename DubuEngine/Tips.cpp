#include "Tips.h"

const char* GetTip(int tip_id) {
	switch (tip_id) {
		case 0: return "TIP: Refill your bladder by drinking water!";
		case 1: return "TIP: Find bones by sniffing!";
		default: return "TIP: I got no tips left for you";
	}
}