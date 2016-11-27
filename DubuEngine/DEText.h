#pragma once
#include <iostream>
#include <string.h>


/* ================================ trim =================================
 *		Trims white spaces off from a string.
 *
 *		Use for string proccessing.
 */
	std::string trim(std::string s);			// String to trim


/* ============================== SplitString =============================
*		Constructs and returns a SubString object.
*		Splits a string by a desired char into array of substrings.
*		The array and number of substrings are stored in a SubStrings structure.
*
*		Use for string proccessing.
*/
	struct SubStrings {
		std::string* s = NULL;					// Array of substrings
		int items = 0;							// Size of the array
	};

	SubStrings SplitString(std::string s,		// String to split
		char c);