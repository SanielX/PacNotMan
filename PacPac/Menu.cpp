#include "Menu.h"
#include "ScreenColors.h"
#include "Screen.h"
#include "InputHandler.h"
#include "GameSettings.h"
#include <fstream>
#include "GameSession.h"
#include "InputHandler.h"

using namespace std;
#undef min
#undef max

void Menu::EnterMenu(int globalTime)
{
	selectedMenuItem = 0;
	lastMenuUpdateTime = globalTime;
}

int Menu::UpdateMenu(Screen* pScreen, int globalTime, InputHandler* pInput)
{
	if (globalTime > lastMenuUpdateTime + 400)
	{
		int inputVec = pInput->GetInputVec();
		pInput->ReleaseInputVec();

		// Decomposing input vector
		int up = (inputVec & (1 << M_UP)) != 0;
		int down = (inputVec & (1 << M_DOWN)) != 0;

		int move = down - up;

		selectedMenuItem += move;
		// Clamp function. Making value >= 0 and <= 2
		selectedMenuItem = min(max(0, selectedMenuItem), 2);

		lastMenuUpdateTime = globalTime;
	}

	// convert const vec2 to vec2 to avoid headache
	vec2 screenSize = vec2(pScreen->GetSizeX(), pScreen->GetSizeY()); 
	pScreen->PrintText(GetCentralRect(GameName, screenSize, 2), 
					  GameName, COL_BRIGHT_YELLOW);

	// Creating a text for each button with 
	// --- TEXT --- thingies around it
	// if selected
	auto startText = ConstructButton(StartGameButton, 
									 selectedMenuItem == 0);
	auto loadText = ConstructButton(LoadCustomMapButton, 
									selectedMenuItem == 1);
	auto exitText = ConstructButton(ExitButton, 
									selectedMenuItem == 2);

	pScreen->PrintText(GetCentralRect(startText, screenSize, 5), startText);
	pScreen->PrintText(GetCentralRect(loadText, screenSize, 6),  loadText);
	pScreen->PrintText(GetCentralRect(exitText, screenSize, 7),  exitText);

	pScreen->PrintText(vec2(screenSize.x - 25, screenSize.y - 10), ControlsTex);

	if (!pInput->IsSpacePressed())
	{
		return 0;
	}

	switch (selectedMenuItem)
	{
	default:
		return 0;  // do nothing
	case 0:
		return MENU_START_GAME;
	case 1:
		return MENU_LOAD_CUSTOM;
	case 2:
		return MENU_EXIT;
	}
}

vec2 Menu::GetCentralRect(string& str, vec2& screenSize, int vert)
{
	return vec2{ (screenSize.x / 2) - ((int)str.size() / 2), vert };
}

string Menu::ConstructButton(string& from, bool isSelected)
{
	if (!isSelected)
	{
		// Don't forget that screen does not clear itself
		string res("    ");
		res += from;
		res += "    ";

		return res;
	}
	else
	{
		string res("--- ");
		res += from;
		res += " ---";

		return res;
	}
}