#pragma once
#include "vec2.h"
#include "Game.h"

enum GameState 
{
	STATE_Menu,
	STATE_InGame,
	STATE_WaitingForRestart,
	STATE_Paused
};

class Menu;

// ќсновний клас в≥кна гри
class PacNotMan
{
public:
	PacNotMan();
	~PacNotMan();

	// WHY NOT BOI
	bool operator()(int deltaTime);

private:
	void StartSession(const char* map);
	void SaveGame();
	void SetState(GameState state);

	void LoadCustomMap();
	bool ValidateMap(string& map);

	static string FileDialogue(LPCWSTR filter = L"All Files (*.*)\0*.*\0",
							   bool openOrSave = true);

	int highScore;
	int globalTime;
	int state = GameState::STATE_Menu;

	const int MAX_MAP_SIZE = 500;

	// ¬каз≥вники а не частина класу
	// так €к ≥накше з'€вл€Їтьс€ циркул€рна залежн≥сть
	GameSession* activeSession;
	InputHandler* input;
	Screen* screen;
	Menu* menu;

	const vec2 SCREEN_OFFSET = vec2{ 5,5 };
	const vec2 SCREEN_SIZE = vec2{ 30, 30 };
	static const char* DEFAULT_MAP;
};

