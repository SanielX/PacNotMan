#pragma once
#include "vec2.h"
#include "InputHandler.h"
#include <string>
#include <Windows.h>

using namespace std;

enum MenuCommand
{
	MENU_EXIT = -1,
	MENU_START_GAME = 1,
	MENU_LOAD_CUSTOM,
};

class GameSession;
class Screen;

// Composition-class responsible for showing stuff in the menu
class Menu
{
public:
	Menu() = default;
	Menu(Menu& other) = default;
	~Menu() = default;

	void EnterMenu(int);
	// Returns enum from MenuCommand or 0 if no action required
	int UpdateMenu(Screen* screen, 
				   int globalTime, InputHandler* input);

private:
	string ConstructButton(string& from, bool isSelected);
	vec2 GetCentralRect(string& str, vec2& screenSize, int vert);

	int lastMenuUpdateTime = 0;
	int selectedMenuItem = 0;

	string GameName = "Pac-NOT-Man";

	string StartGameButton = "Start";
	string LoadCustomMapButton = "Load Custom Map";
	string ExitButton = "Exit";

	string ControlsTex = "-Press \"Space\" To Select\n"
		"-Use Arrows to navigate\n-\"F6\" "
		"To save the game\n-\"ESC\" to pause\n"
		"During the game: \nPress f6 to save.\n";
};

