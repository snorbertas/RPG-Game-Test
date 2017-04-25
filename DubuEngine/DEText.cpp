#include <sstream>
#include "DEText.h"
using namespace std;

string trim(string s) {
	return s.erase(s.find_last_not_of(" \n\r\t") + 1);
}