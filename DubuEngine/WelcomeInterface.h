#pragma once
#include <string>
#include <vector>
#include <allegro5/allegro_font.h>
#include "Sprites.h"

class Game;

// Extra
struct WIMisc {
	// Types of misc
	enum Type { DIG_KEY, DRINK_KEY, PEE_KEY, UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY };

	// Variables
	int x, y;
	Type type;

};

// Button
struct WIButton {
	int x, y;
	int id;
	bool init;
};

// Line of text
struct WIText {
	int x, y;
	int font_type;
	int r, g, b;
	int align;
	std::string text;
};

// Local image
struct WIImage {
	int x, y;
	string type;
	int id;
};

// Online image
struct WIImageO {
	int x, y;
	ALLEGRO_BITMAP* img;

	/* The local image should stay, but the online image needs to be
	   deleted from memory when destroyed */
	// TODO: Make sure this gets called when deleting a Welcome Interface
	~WIImageO() { al_destroy_bitmap(img); } 
};

// Full Interface
class WelcomeInterface {
public:
	// Constructor
	WelcomeInterface() : _x(0), _y(0), _w(18), _h(18) {}

	// Constructor
	WelcomeInterface(int x, int y, int w, int h) : _x(x), _y(y), _w(w), _h(h) {}

	// Proccess the raw text to create vectors of text and images
	void InterpretAllRawText();

	// Load code from a file
	void LoadTextFromFile(const char* file_path);

	// Set the raw text to a new value
	void SetText(const char* text) { _raw_text = text; };

	// Add more raw text
	void AddText(const char* text) { _raw_text += text; };

	// Reset the raw text
	void ResetText() { _raw_text = ""; };

	// Reset special
	void ResetSpecial(Game* g);

	// Reset whole interface
	void ResetInterface() {
		ResetText();
		_line.clear();
		_image.clear();
		_image_online.clear();
		_button.clear();
		_misc.clear();
	};

	// Reset whole interface including game elements influenced by this interface
	void ResetInterface(Game* g) {
		ResetText();
		ResetSpecial(g);
		_line.clear();
		_image.clear();
		_image_online.clear();
		_button.clear();
		_misc.clear();
	};

	// Set coordinates and dimensions
	void SetDimensions(int x, int y, int w, int h) {
		_x = x;
		_y = y;
		_w = w;
		_h = h;
	}

	// Handle a tick (timer)
	void Handle() {
		timer++;
		if (timer > 60) timer = 0;
	}

	// Initialize special EXTRA elements (buttons, animations, etc.)
	void InitSpecialElements(Game* g);

	// Draw all interpreted elements
	void DrawAllElements(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font);

	// Retrieve starting X point
	int X() { return _x; };

	// Retrieve starting Y point
	int Y() { return _y; };

	// Retrieve Width
	int Width() { return _w; };

	// Retrieve Height
	int Height() { return _h; };

	// Timer used for animations, etc.
	int timer = 0;

private:
	int _x, _y, _w, _h;				// The square within the interface is allowed to draw
	std::string _raw_text = "";		// The raw text used for interpretation

	// Vectors of elements that were interpreted from raw text
	std::vector<WIText> _line;
	std::vector<WIImage> _image;
	std::vector<WIImageO> _image_online;
	std::vector<WIButton> _button;
	std::vector<WIMisc> _misc;
};