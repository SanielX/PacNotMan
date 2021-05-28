#include "Game.h"
#include "AsciiColor.h"
#include <stdexcept>
#include <assert.h>
using std::string;

Screen::~Screen()
{
	for (int i = 0; i < height; i++)
	{
		delete[] pixels[i];
		delete[] colors[i];
	}

	delete[] pixels;
	delete[] colors;
	delete[] buf;
}

Screen::Screen(int x, int y)
{
	width = x;
	height = y;
	buf = new char[(width * height) * 21];  // 20 is the max length of a single color + 1 char per pixel
	pixels = new char* [y];
	colors = new AsciiColor * [y];

	for (size_t i = 0; i < y; i++)
	{
		pixels[i] = new char[x];
		colors[i] = new AsciiColor[x];
		for (size_t j = 0; j < x; j++)
		{
			pixels[i][j] = ' ';
			colors[i][j] = AsciiColor();
		}
	}

	// Ansii color codes are not supported on windows by default
	// So we need to enable it manually
	DWORD l_mode;
	auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
	auto mode = GetConsoleMode(handle, &l_mode);
	SetConsoleMode(handle, l_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void SetConsoleCursorPos(int x, int y)
{
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Some special case functions to print strings to screen, since
// screen is composed of "pixels" and you can't just cout stuff
void Screen::PrintText(vec2 xy, const string& line, const AsciiColor& color)
{
	int startX = xy.x;
	int linesLength = 0;

	for (int i = 0; i < line.length(); i++)
	{
		vec2 pos = vec2(xy.x + i - linesLength, xy.y);
		char c = line[i];
		if (c == '\n') {
			xy.y++;
			linesLength += xy.x;
			xy.x = startX;
			continue;
		}

		if (BoundCheck(pos))
		{
			pixels[pos.y][pos.x] = c;
			colors[pos.y][pos.x] = color;
		}
		else
		{
			break;
		}
	}
}

// Prints test like
// H
// E
// Y
void Screen::PrintTextVertical(vec2 xy, const string& line, AsciiColor color)
{
	for (int i = 0; i < line.length(); i++)
	{
		vec2 pos = vec2{ xy.x, xy.y + i };
		char c = line[i];

		if (BoundCheck(pos))
		{
			pixels[pos.y][pos.x] = c;
			colors[pos.y][pos.x] = color;
		}
		else
		{
			break;
		}
	}
}

void Screen::PrintText(vec2 xy, const string& line)
{
	int startX = xy.x;
	int lastI = 0;

	for (int i = 0; i < line.length(); i++)
	{
		vec2 pos = vec2{ (xy.x + i - lastI), xy.y };
		char c = line[i];
		if (c == '\n') {
			xy.y++;
			xy.x = startX;
			lastI = i + 1;
			continue;
		}

		SetPixel(pos, c);
	}
}

bool Screen::BoundCheck(const vec2& xy)
{
	return (xy.x >= 0 && xy.x < width && xy.y >= 0 && xy.y < height);
}

void Screen::SetPixel(vec2 xy, char value)
{
	if (!BoundCheck(xy))
		return;

	pixels[xy.y][xy.x] = value;
}

void Screen::SetColor(vec2 xy, const AsciiColor& col)
{
	if (!BoundCheck(xy))
		return;

	colors[xy.y][xy.x] = col;
}

void Screen::Draw()
{
	// Also we just overwrite whatever was 
	// in console each "frame"
	SetConsoleCursorPos(0, 0);
	int i = 0;

	AsciiColor lastColor = COL_RESET;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// Adding color to buffer before inserting a char
			// But only do that if color has CHANGED
			if (colors[y][x] != lastColor || pixels[y][x] != ' ')
			{
				int j = 0;

				while (colors[y][x].colorPtr[j] != '\0')
				{
					buf[i++] = colors[y][x].colorPtr[j];
					j++;
				}

				lastColor = colors[y][x];
			}

			// Btw i will be read first and then added, 
			// not the other way around
			// use ++i to do it, but don't do it here
			buf[i++] = pixels[y][x];
		}

		buf[i++] = '\n';
	}

	// Okay so the main problem with using cout 
	// for each character is that it flickers as hell,
	// And as I read here:
	// https://stackoverflow.com/questions/18412164/fast-c-string-output
	// fwrite is much faster
	fwrite(buf, sizeof(char), i, stdout);
}

void Screen::SetFontSize(int x, int y)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = x;                   // Width of each character in the font
	cfi.dwFontSize.Y = y;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void Screen::Clear()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			pixels[y][x] = ' ';
			colors[y][x] = COL_RESET;
		}
	}
}

int Screen::GetSizeY() {
	return height;
}

int Screen::GetSizeX() {
	return width;
}


////////////////////////
//					  //
//      ITERATOR      //
//				      //
////////////////////////


Screen::iterator::iterator()
	: iterator(NULL, 0, 0) { }

Screen::iterator::iterator(Screen* s, int x, int y)
{
	screen = s;
	indexX = x;
	indexY = y;
}

Screen::iterator Screen::begin()
{
	auto iter = Screen::iterator(this, 0, 0);
	return iter;
}

Screen::iterator Screen::end()
{
	auto iter = Screen::iterator(this, width - 1, height - 1);
	return iter;
}

Screen::pixel Screen::iterator::operator*()
{
	assert(screen != NULL);

	Screen::pixel p;
	auto y = this->screen->GetSizeY();

	// There are no \n in screen, so we need to add them ourselves by
	// extending width range of the screen
	if (indexX == screen->width)
	{
		p.c = '\n';
		p.color = COL_RESET;
	}
	else if (!screen->BoundCheck(vec2(indexX, indexY)))
			throw std::invalid_argument("Index out of bounds");
	else
	{
		p.c = screen->pixels[indexY][indexX];
		p.color = screen->colors[indexY][indexX];
	}

	return p;
}

bool Screen::iterator::operator==(const Screen::iterator& other) {
	return this->indexX == other.indexX &&
		   this->indexY == other.indexY &&
		   screen == other.screen;
}

bool Screen::iterator::operator!=(const Screen::iterator& other) {
	return this->indexX != other.indexX ||
		   this->indexY != other.indexY ||
		         screen != other.screen;
}

void Screen::iterator::operator++(int)
{
	assert(screen != NULL);
	indexX++;
	if (indexX > screen->width)
	{
		indexX = 0;
		indexY++;
	}
}

void Screen::iterator::operator--(int)
{
	assert(screen != NULL);
	indexX--;
	if (indexX < 0)
	{
		indexX = screen->width - 1;
		indexY--;
	}
}
