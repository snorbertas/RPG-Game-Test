#pragma once
#include <string>
#include <vector>
#include <allegro5/allegro_font.h>
#include "Sprites.h"
class Game;

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

	// Reset whole interface
	void ResetInterface() {
		ResetText();
		_line.clear();
		_image.clear();
		_image_online.clear();
	};

	// Set coordinates and dimensions
	void SetDimensions(int x, int y, int w, int h) {
		_x = x;
		_y = y;
		_w = w;
		_h = h;
	}

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

private:
	int _x, _y, _w, _h;				// The square within the interface is allowed to draw
	std::string _raw_text = "";	// The raw text used for interpretation

	// Vectors of elements that were interpreted from raw text
	std::vector<WIText> _line;
	std::vector<WIImage> _image;
	std::vector<WIImageO> _image_online;
};