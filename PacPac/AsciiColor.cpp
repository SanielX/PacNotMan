#include "AsciiColor.h"
#include "ScreenColors.h"
#include "GameMap.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <functional>

// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
const char* COL_RESET		   = "\033[0m"; 
const char* COL_RED			   = "\033[38;5;196m";
const char* COL_YELLOW	       = "\033[38;5;226m";
const char* COL_BLUE		   = "\033[34m";     
const char* COL_MAGENTA		   = "\033[35m";     
const char* COL_CYAN		   = "\033[36m";     
const char* COL_WHITE		   = "\033[37m";     
const char* COL_BRIGHT_YELLOW  = "\033[93m";     
const char* COL_BRIGHT_BLUE    = "\033[94m";     
const char* COL_BRIGHT_MAGNETA = "\033[38;5;213m";     
const char* COL_BRIGHT_CYAN    = "\033[38;5;123m";     
const char* COL_ORANGE		   = "\033[38;5;222m";   

bool CompareText(const char* a, const char* b) 
{
	return strcmp(a, b) == 0;
}

AsciiColor::AsciiColor(const AsciiColor& other)
: AsciiColor(other.colorPtr) { }

AsciiColor::AsciiColor()
: AsciiColor(COL_RESET) { }

AsciiColor::AsciiColor(const char* col) 
{
	assert(col[0] == '\033'); // The value is not an escape code
	assert(strlen(col) <= 20);  // The value is too long

	colorPtr = CopyText(col);
}

AsciiColor::~AsciiColor() 
{
	delete[] colorPtr;
}

void AsciiColor::operator=(const AsciiColor& col) 
{
	delete[] colorPtr;
	colorPtr = CopyText(col.colorPtr);
}

bool AsciiColor::operator==(const AsciiColor& other) {
	return CompareText(colorPtr, other.colorPtr);
}

bool AsciiColor::operator!=(const AsciiColor& other) {
	return !CompareText(colorPtr, other.colorPtr);
}