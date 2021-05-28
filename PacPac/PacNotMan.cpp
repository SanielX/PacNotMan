#include <fstream>
#include <string>
#include <assert.h>

#include "PacNotMan.h"
#include "Game.h"
#include "GameSettings.h"
#include "Menu.h"
#include "user_exception.h"

using namespace std;

const char* PacNotMan::DEFAULT_MAP =
"###################\n"
"#........#........#\n"
"#^##.###.#.###.##^#\n"
"#.##.###.#.###.##.#\n"
"#.................#\n"
"#.##.#.#####.#.##.#\n"
"#....#...#...#....#\n"
"####.### # ###.####\n"
"   #.#   B   #.#   \n"
"####.# ##W## #.####\n"
"    .  #CIP#  .    \n"
"####.# ##### #.####\n"
"   #.#       #.#   \n"
"####.# ##### #.####\n"
"#........#........#\n"
"#.##.###.#.###.##.#\n"
"#^.#.....@.....#.^#\n"
"##.#.#.#####.#.#.##\n"
"#....#...#...#....#\n"
"#.######.#.######.#\n"
"#.................#\n"
"###################\0";

PacNotMan::PacNotMan()
{
	activeSession = nullptr;
	globalTime = 0;
	highScore = 0;
	screen = new Screen(SCREEN_SIZE.x, SCREEN_SIZE.y);
	screen->SetFontSize(15, 15);
	input = new InputHandler();
	menu = new Menu();
}

PacNotMan::~PacNotMan()
{
	delete screen;
	delete input;
	delete menu;

	// Active session does exist 
	if (activeSession != nullptr)
		delete activeSession;
}

bool PacNotMan::operator()(int deltaTime)
{
	globalTime += deltaTime;

	// We'll use this string for additional text later
	string txt; 
	int newScore = 0;
	bool gOver = false;

	input->Update();

	switch (state)
	{
	case STATE_Menu:
	{
		int menuResult;
		menuResult = menu->UpdateMenu(screen, globalTime, input);
		if (menuResult == MENU_EXIT)
			return true;  // true means exit the game

		if (menuResult == MENU_START_GAME)
		{
			StartSession(DEFAULT_MAP);
			return false;
		}

		if (menuResult == MENU_LOAD_CUSTOM)
		{
			try
			{
				LoadCustomMap();
			}
			catch (user_exception& invalidMap)
			{
				invalidMap.ShowError();
			}

			return false;
		}
	}
	break;
	case STATE_InGame:
	{
		gOver = activeSession->Update(deltaTime);
		newScore = activeSession->GetScore();

		if (input->IsF6Pressed())
		{
			try
			{
				SaveGame();
			}
			catch (user_exception& userException)
			{
				userException.ShowError();
			}
		}

		if (newScore > highScore)
			highScore = newScore;

		// High score is drawn by outside code,
		// because GameLoop does not know
		// what current high score is
		txt = "High Score:";
		txt += std::to_string(highScore);
		vec2 mapSize = activeSession->GetMap()->GetMapSize();
		vec2 offset = SCREEN_OFFSET;
		screen->PrintText(offset + vec2{ (mapSize.x / 2) + 1, -1 }, txt);

		if (gOver)
		{
			SetState(STATE_WaitingForRestart);
		}
		else if (input->IsEscapePressed())
		{
			SetState(STATE_Paused);
		}
	}
	break;
	case STATE_WaitingForRestart:
	{
		activeSession->DrawToScreen();

		txt = "You lost. Space to retry\nEscape to exit to menu";
		vec2 mapSize = activeSession->GetMap()->GetMapSize();
		vec2 offset = SCREEN_OFFSET;
		vec2 drawPos = offset + vec2{ 0, mapSize.y + 1 };
		screen->PrintText(drawPos, txt);

		if (input->IsEscapePressed())
		{
			SetState(STATE_Menu);
		}
		else if (input->IsSpacePressed())
		{
			screen->Clear();
			activeSession->RestartSession();
			SetState(STATE_InGame);
		}
	}
	break;
	case STATE_Paused:
	{
		txt = "Tha game is paused\nPress ESC to enter menu"
			  "\nSpace to continue";
		vec2 mapSize = activeSession->GetMap()->GetMapSize();
		mapSize.x = 0;
		vec2 offset = SCREEN_OFFSET;
		vec2 drawPos = offset + mapSize + vec2{ 0, 1 };
		screen->PrintText(drawPos, txt);

		if (input->IsEscapePressed())
		{
			SetState(STATE_Menu);
		}
		else if (input->IsSpacePressed())
		{
			SetState(STATE_InGame);
		}
	}
	break;
	}

	screen->Draw();
	return false;
}

string PacNotMan::FileDialogue(LPCWSTR filter, bool openOrSave)
{
	// Відкриває діалог Windows, який повертає адрес до
	// створенного або відкритого файла
	OPENFILENAME ofn;
	WCHAR fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";
	string fileNameStr;
	if (openOrSave) {
		if (GetOpenFileName(&ofn))
		{
			wstring ws(fileName);
			fileNameStr = string(ws.begin(), ws.end());
		}
	}
	else {
		if (GetSaveFileName(&ofn))
		{
			wstring ws(fileName);
			fileNameStr = string(ws.begin(), ws.end());
		}
	}
	return fileNameStr;
}

void PacNotMan::SaveGame()
{
	assert(activeSession != nullptr);

	auto path = FileDialogue(L"Text Files (*.txt)\0*.txt\0", false);

	ofstream stream(path);
	if (!stream.is_open())
	{
		throw user_exception("File path in incorrect");
	}

	stream << "Your game results:\n";
	stream << "Highest Score: " << highScore << endl;
	stream << "Last Score: " << activeSession->GetScore() << endl;
	stream << "Last Level: " << activeSession->GetLevel() << endl;
	stream << "Snapshot of the map (To see it properly "
		"you need to open file with 437 encoding): \n";

	// Using iterator of screen to output everything to file
	for (auto i = screen->begin(); i != screen->end(); i++)
	{
		auto end = screen->end();
		char c = (*i).c;
		stream << c;
	}

	stream.close();
}

void PacNotMan::LoadCustomMap()
{
	string map;
	auto path = FileDialogue(L"Text Files (*.txt)\0*.txt\0");

	fstream str;
	str.open(path, ios::binary | ios::in);

	if (!str.is_open())
	{
		// It'll display nice windows message in case 
		// something is wrong with the file
		throw user_exception("File is invalid");
	}

	getline(str, map, '\0');
	ValidateMap(map);

	StartSession(map.c_str());
}

bool PacNotMan::ValidateMap(string& map)
{
	if (map.size() > MAX_MAP_SIZE)
	{
		throw user_exception("File is too big");
	}

	// Перевірка мапи на слідкуванням правилам

	auto hasPlayer = map.find('@') != string::npos;
	if (!hasPlayer)
	{
		throw user_exception("Map has no player");
	}

	// Без будинку привидів, штучний інтелект не буде працювати
	auto hasGhostWall = map.find('W') != string::npos;
	if (!hasGhostWall)
	{
		throw user_exception("No ghost house on map");
	}

	// Поведінка Інкі залежить від Блінки, тож якщо він не присутній
	// мапа є неправильною
	auto hasInky = map.find('I') != string::npos;
	if (hasInky && map.find('B') == string::npos)
	{
		throw user_exception("Map has Inky but no Blinky is found."
			"Ghost behvaiour will be invalid");
	}

	return true;
}

void PacNotMan::SetState(GameState state)
{
	switch (state)
	{
	default:
		screen->Clear();
		break;
	case STATE_Menu:
		if (activeSession != nullptr) {
			delete activeSession;
			activeSession = nullptr;
		}

		screen->Clear();
		menu->EnterMenu(globalTime);
		break;
	case STATE_Paused:  // Pausing should not clear whole screen
		break;
	}

	this->state = state;
}

void PacNotMan::StartSession(const char* map)
{
	screen->Clear();
	SetState(STATE_InGame);
	activeSession = new GameSession(map, screen, input);
	activeSession->SetDrawOffset(SCREEN_OFFSET);
}