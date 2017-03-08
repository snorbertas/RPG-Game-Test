#include <sstream>
#include <fstream>
#include "Game.h"
#include "ScaledDraw.h"
#include "DEText.h"
#include "Interfaces.h"

void WelcomeInterface::ResetSpecial(Game* g) {
	// Hide the 10 buttons in the interface
	for (int i = 0; i < 10; i++) {
		g->Buttons[17 + i].visible = false;
	}
}

void WelcomeInterface::InitSpecialElements(Game* g) {
	// Initialize buttons
	for (size_t i = 0; i < _button.size(); i++) {
		if (!_button[i].init) {
			int button_id = _button[i].id;
			g->Buttons[17 + button_id].x = _button[i].x + _x;
			g->Buttons[17 + button_id].y = _button[i].y + _y;
			g->Buttons[17 + button_id].visible = true;
			_button[i].init = true;
		}
	}
}

void WelcomeInterface::DrawAllElements(Game* g, SpriteStruct* sprites, ALLEGRO_FONT** font) {
	// Horizontal/Vertical ratios for text drawing
	float r_x = (float)g->s_x / (float)g->BWIDTH;
	float r_y = (float)g->s_y / (float)g->BHEIGHT;

	// Draw all text
	for (size_t i = 0; i < _line.size(); i++) {
		DrawText(font[_line[i].font_type],
			_line[i].r, _line[i].g, _line[i].b,
			_line[i].x + _x, _line[i].y + _y,
			_line[i].align,
			_line[i].text.c_str());
	}

	// Draw all local images
	for (size_t i = 0; i < _image.size(); i++) {
		if (_image[i].type == "Body") {
			DrawImage(g, sprites->img_body[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "Interface") {
			DrawImage(g, sprites->img_interface[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "GFX") {
			DrawImage(g, sprites->img_gfx[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "Tile") {
			DrawImage(g, sprites->img_tile[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "Icon") {
			DrawImage(g, sprites->img_icon[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "Sprite") {
			DrawImage(g, sprites->img_sprite[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "Background") {
			DrawImage(g, sprites->img_background[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else if (_image[i].type == "Object") {
			DrawImage(g, sprites->img_object[_image[i].id],
				_image[i].x + _x, _image[i].y + _y, 0);
		} else {
			DrawText(font[2], 255, 0, 0, _image[i].x + _x, _image[i].y + _y, ALLEGRO_ALIGN_LEFT, "Error: Wrong Image Type: %s:%i", _image[i].type, _image[i].id);
		}
	}

	// Draw all online images
	for (size_t i = 0; i < _image_online.size(); i++) {
		DrawImage(g, _image_online[i].img,
			_image_online[i].x + _x, _image_online[i].y + _y, 0);

	}

	// Draw misc
	for (size_t i = 0; i < _misc.size(); i++) {
		int anim = 0;
		int sprite_id = SPRITE_BUTTON_BLANK_SMALL;
		int x_offset = 0;
		int x_offset_text = 18;
		if (timer > 30) anim = 1;
		if (_misc[i].type <= WIMisc::Type::RIGHT_KEY) {
			string key_str = "ERR";
			if (_misc[i].type == WIMisc::Type::PEE_KEY) key_str = GetKeyName(g->keys.pee_bind);
			if (_misc[i].type == WIMisc::Type::DIG_KEY) key_str = GetKeyName(g->keys.dig_bind);
			if (_misc[i].type == WIMisc::Type::DRINK_KEY) key_str = GetKeyName(g->keys.drink_bind);
			if (_misc[i].type == WIMisc::Type::UP_KEY) key_str = GetKeyName(g->keys.up_bind);
			if (_misc[i].type == WIMisc::Type::DOWN_KEY) key_str = GetKeyName(g->keys.down_bind);
			if (_misc[i].type == WIMisc::Type::LEFT_KEY) key_str = GetKeyName(g->keys.left_bind);
			if (_misc[i].type == WIMisc::Type::RIGHT_KEY) key_str = GetKeyName(g->keys.right_bind);
			if (key_str.length() > 2) {
				sprite_id = SPRITE_BUTTON_BLANK_MED;
				x_offset = -5;
				x_offset_text = 23;
			}
			DrawImage(g, sprites->img_button[sprite_id + anim],
				_misc[i].x + x_offset + _x,
				_misc[i].y + _y + (anim * 4),
				0, 0.8);
			DrawText(font[2], 0, 0, 0,
				_misc[i].x + x_offset + x_offset_text + _x,
				_misc[i].y + 6 + _y + (anim * 4),
				ALLEGRO_ALIGN_CENTER, "%s", key_str.c_str());
		}
	}
}

int HexToInt(char hex) {
	if (hex == '1') return 1;
	if (hex == '2') return 2;
	if (hex == '3') return 3;
	if (hex == '4') return 4;
	if (hex == '5') return 5;
	if (hex == '6') return 6;
	if (hex == '7') return 7;
	if (hex == '8') return 8;
	if (hex == '9') return 9;
	if (hex == 'A') return 10;
	if (hex == 'B') return 11;
	if (hex == 'C') return 12;
	if (hex == 'D') return 13;
	if (hex == 'E') return 14;
	if (hex == 'F') return 15;
	return 0;
}

struct RGBColor {
	RGBColor() {};
	RGBColor(int r, int g, int b) : r(r), g(g), b(b) {}
	int r = 255;
	int g = 255;
	int b = 255;
};

RGBColor HexToRGB(string hex) {
	RGBColor color;
	if (hex.length() != 6) return RGBColor(255, 255, 255);

	// Red, Green, Blue
	color.r = ((HexToInt(hex[0]) * 16) + HexToInt(hex[1]));
	color.g = ((HexToInt(hex[2]) * 16) + HexToInt(hex[3]));
	color.b = ((HexToInt(hex[4]) * 16) + HexToInt(hex[5]));

	return color;
}

void WelcomeInterface::InterpretAllRawText() {
	// Create a stream to read the raw text
	istringstream iss(_raw_text);
	while (!iss.eof()) {
		// One line at a time
		string current_line;
		getline(iss, current_line);

		// Setting variables for this line
		int align = ALLEGRO_ALIGN_LEFT;
		int x = -1;
		int y = 10;
		int font_type = 0;
		int r = 0;
		int g = 0;
		int b = 0;
		int button_id = -1;
		int misc_id = -1;
		string text = "";
		string img_type = "";
		string img_id = "";
		string img_url = "";

		// Find all codes
		size_t pos = 0;
		size_t text_start = 0;
		while (pos != string::npos){
			size_t found_start = current_line.find("[", pos);
			size_t found_end = current_line.find("]", found_start);

			// Check if we found a code
			if (found_start != string::npos && found_end != string::npos) {
				string code = current_line.substr(found_start + 1, (found_end - found_start) - 1);

				// Now analyze what sort of code we've found
				if (code.substr(0, 5) == "misc=") {
					// Add misc
					misc_id = atoi(code.substr(5, code.length() - 5).c_str());
					if (misc_id >= 10) misc_id = -1;

				} else if (code.substr(0, 7) == "button=") {
					// Add button
					button_id = atoi(code.substr(7, code.length() - 7).c_str());
					if (button_id >= 10) button_id = -1;

				} else if (code.substr(0, 5) == "text=") {
					// Add text
					text = code.substr(5, code.length() - 5);

				} else if (code.substr(0, 5) == "font=") {
					// Change font
					font_type = atoi(code.substr(5, code.length() - 5).c_str());

				} else if (code.substr(0, 6) == "color=") {
					// Change color
					string setting = code.substr(7, 13);
					RGBColor temp = HexToRGB(setting);
					r = temp.r;
					g = temp.g;
					b = temp.b;

				} else if (code.substr(0, 4) == "img=") {
					// Add image
					if (code.substr(4, 4) == "http") {
						// Online image
						img_url = code.substr(4, code.length() - 4);

					} else {
						// Local image
						string img_loc = code.substr(4, code.length() - 4);

						// Split by the : symbol
						int split_pos = img_loc.find(':');
						if (split_pos != string::npos) {
							img_type = img_loc.substr(0, split_pos);
							img_id = img_loc.substr(split_pos + 1, img_loc.length() - split_pos - 1);
						}
					}
				} else if (code.substr(0, 2) == "x=") {
					// Set x coordinate
					x = atoi(code.substr(2, code.length() - 2).c_str());

				} else if (code.substr(0, 2) == "y=") {
					// Set y coordinate
					y = atoi(code.substr(2, code.length() - 2).c_str());

				} else if (code.substr(0, 4) == "left") {
					// Align left
					align = ALLEGRO_ALIGN_LEFT;

					// If x is not specified, will make it = 10
					if(x == -1) x = 10;

				} else if (code.substr(0, 6) == "center") {
					// Align center
					align = ALLEGRO_ALIGN_CENTER;

					// If x is not specified, will center it
					if (x == -1) x = _w / 2;

				} else if (code.substr(0, 5) == "right") {
					// Align right
					align = ALLEGRO_ALIGN_RIGHT;

					// If x is not specified, will make it max
					x = _w - 10;
				} else if (code.substr(0, 3) == "bot") {
					// Set max y value;
					y = _h - 30;
				}
			}

			// Update position
			pos = found_end;
		}

		// Correct/assume some values
		if (x == -1 && align == ALLEGRO_ALIGN_LEFT) x = 10;
		if (x == -1 && align == ALLEGRO_ALIGN_CENTER) x = _w /2;
		if (x == -1 && align == ALLEGRO_ALIGN_RIGHT) x = _w - 10;
		if (x < 10) x = 10;
		if (x > _w - 10) x = _w - 10;
		if (y < 10) y = 10;
		if (y > _h - 30) y = _h - 30;

		// First decide the type of element
		if (misc_id != -1) {
			// It's misc
			WIMisc temp_WIMisc;
			temp_WIMisc.type = (WIMisc::Type)misc_id;
			temp_WIMisc.x = x;
			temp_WIMisc.y = y;
			_misc.push_back(temp_WIMisc);

		} else if (button_id != -1) {
			// It's a button
			WIButton temp_WIButton;
			temp_WIButton.x = x;
			temp_WIButton.y = y;
			temp_WIButton.id = button_id;
			temp_WIButton.init = false;
			_button.push_back(temp_WIButton);

		} else if (text != "") {
			// It's text
			WIText temp_WIText;
			temp_WIText.text = text;
			temp_WIText.font_type = font_type;
			temp_WIText.align = align;
			temp_WIText.r = r;
			temp_WIText.g = g;
			temp_WIText.b = b;
			temp_WIText.x = x;
			temp_WIText.y = y;

			// Add it to the vector so it's ready to be drawn
			_line.push_back(temp_WIText);

		} else {
			// It's an image
			if (img_type != "") {
				// Local image
				WIImage temp_WIImage;
				temp_WIImage.type = img_type;
				temp_WIImage.id = atoi(img_id.c_str());
				temp_WIImage.x = x;
				temp_WIImage.y = y;
				_image.push_back(temp_WIImage);

			} else if(img_url != ""){
				// Online image
				// 1. Download
				// 2. Load it
				// 3 Delete the local image
				// 4. Add it to vector
			}
		}
	}
}

void WelcomeInterface::LoadTextFromFile(const char* file_path) {
	if (al_filename_exists(file_path)) {
		string line;
		ifstream data_file(file_path);
		if (data_file.is_open()){
			while (getline(data_file, line)) {
				AddText(line.c_str());
				AddText("\n");
			}
			data_file.close();
		} else {
			cout << "Couldn't open file: " << file_path << endl;
		}
	} else {
		cout << "Couldn't find file: " << file_path << endl;
	}
}