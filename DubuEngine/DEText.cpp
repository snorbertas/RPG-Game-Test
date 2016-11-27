#include <sstream>
#include "DEText.h"
using namespace std;

string trim(string s) {
	return s.erase(s.find_last_not_of(" \n\r\t") + 1);
}

SubStrings SplitString(string s, char c) {
	SubStrings result;

	// Count the items
	istringstream words_counter(s);
	int count = 0;
	string temp;
	while (words_counter >> temp) {
		count++;
	}
	result.items = count;

	// Write the items into substrings
	string* sub_strings = new string[count + 1];
	istringstream words(s);
	int word_id = 0;
	while (words >> temp) {
		sub_strings[word_id] = temp;
		word_id++;
	}
	result.s = sub_strings;

	// Return the resulting substring
	return result;
}