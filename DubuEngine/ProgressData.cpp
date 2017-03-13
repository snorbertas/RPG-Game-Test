#include <fstream>
#include <allegro5/allegro.h>
#include "ProgressData.h"
using namespace std;

void ProgressData::Save(const char* file_path) {
	ofstream level_file(file_path, ios::binary);
	level_file.write((char*)this, sizeof(*this));
	level_file.close();
}

void ProgressData::Load(const char* file_path) {
	if (al_filename_exists(file_path)) {
		ifstream level_file(file_path, ios::binary);
		level_file.read((char*)this, sizeof(*this));
		level_file.close();
	}
}