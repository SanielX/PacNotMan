#pragma once
#include "Game.h"
#include "InputHandler.h"
#include "GameMap.h"

using std::vector;
using std::map;
using std::set;

const int drawTickRate = 50;

class Player;
class AGhost;

// написал на инглише переводить лень
// game sessions manages updates of every system
// and also manages player, which could be turned into a separate
// class, but it would actually complicate code much more
// than I would like, so we'll go with mega class
class GameSession
{
public:
	GameSession(const char* map, Screen* screen, InputHandler* input);
	~GameSession();

	bool Update(int deltaTime);
	void RestartSession();
	void SetDrawOffset(vec2 value);
	int  DotsLeft();
	int  GetLevel();
	int  GetScore();

	vec2 GetGhostPosition(int name);
	vec2 GetPlayerPosition();
	vec2 GetPlayerDirection();

	void WrapPos(vec2& pos);

	GameMap* GetMap();

	void DrawToScreen();

private:
	void CheckCollisions();
	void SetupLevel();
	bool RespawnAll();

	Screen* screen;
	InputHandler* input;
	GameMap* map;

	vector<AGhost*> ghosts;
	Player* player;

	vec2 drawOffset;

	bool wantRespawn;
	bool wantNewLevel;
	bool drawReadySign;

	int livesLeft;
	int pauseTime; // Used when eating ghost, or when starting new life
	int time;

	int score;
	unsigned level;
};

