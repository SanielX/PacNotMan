#pragma once
#include "vec2.h"
#include "ScreenColors.h"
#include "AsciiColor.h"
#include <string>

using std::string;

const char BLOCK_CHAR = 219;
const char POWER_UP_CHAR = 'o';
const char PLAYER_CHAR = '@';
const char DOT_CHAR = '.';

class Screen
{
public:
	Screen(int x, int y);
	void SetFontSize(int x, int y);

	void SetPixel(vec2 xy, char value);
	void SetColor(vec2 xy, const AsciiColor& value);

	void PrintText(vec2 xy, const string& line);
	void PrintText(vec2 xy, const string& line, const AsciiColor& col);
	void PrintTextVertical(vec2 xy, const string& line, AsciiColor col);

	void Draw();
	void Clear();

	int GetSizeX();
	int GetSizeY();

	~Screen();

	struct pixel 
	{
		char c;
		AsciiColor color;
	};

	class iterator 
	{
	public:
		iterator();
		iterator(Screen* s, int, int);

		void operator++(int);
	
		void operator--(int);

		bool operator==(const iterator& i);
		bool operator!=(const iterator& i);

		Screen::pixel operator*();
	private:
		Screen* screen;
		int indexX, indexY;
	};

	iterator begin();
	iterator end();
private:
	bool BoundCheck(const vec2& pos);
	int width, height;
	char* buf;

	char** pixels;
	AsciiColor** colors;
};

