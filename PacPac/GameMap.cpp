#include <assert.h>
#include <stdexcept>
#include "InputHandler.h"
#include "GameMap.h"
#include "AGhost.h"
#include "AsciiColor.h"
#include "Screen.h"
#include "GameSettings.h"
#include "Inky.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Clyde.h"

#define ADD_GHOST(type) { type* _ghost; \
						 _ghost = new type(mySession, pos);\
						 ghosts.push_back(_ghost);\
						 _ghost->Sleep(AGhost::GhostSleepTime(NAME_##type, lvl)); }

char* CopyText(const char* txt)
{
	auto length = strlen(txt);
	char* copy = new char[length + 1];
	strcpy_s(copy, sizeof(char) * (length + 1), txt);
	return copy;
}

GameMap::GameMap(const char* map)
{
	this->gameMap = CopyText(map);
	mapSizeX = 0; mapSizeY = 0;
}

void GameMap::Setup(int lvl, GameSession* mySession,
	vec2& player, vector<AGhost*>& ghosts)
{
	walls.clear();
	dots.clear();
	powerUps.clear();

	int i = 0, x = 0, y = 0;
	while (gameMap[i] != '\0')
	{
		vec2 pos = vec2(x, y);
		switch (gameMap[i])
		{
		case '\r':  // Just ignore \r symbol
			++i;
			continue;
			break;
		case '@':
			player = pos;
			break;
		case '#':
			walls.insert(pos);
			break;
		case '.':
			dots.insert(pos);
			break;
		case '^':
			powerUps.insert(pos);
			break;
		case 'W':
			ghostGate = pos;
			break;

		case 'B':
			// Macro to add ghost by type
			ADD_GHOST(Blinky);
			break;
		case 'P':
			ADD_GHOST(Pinky);
			break;
		case 'I':
			ADD_GHOST(Inky);
			break;
		case 'C':
			ADD_GHOST(Clyde);
			break;

		case '\n':
			if (mapSizeX == 0)
				mapSizeX = x;
			y++;
			x = -1; // There is x++ next
			break;
		}

		x++;
		i++;
	}
	mapSizeY = y + 1;

	BakeWalls();
}

#undef ADD_GHOST

// This map was made by creating 3x3 matrix, then
// Assign to each side a power of 2
// Making each turn guaranteed to be unique
std::unordered_map<int, char> GameMap::wallMap
{
	// Turn right
	{ 10, 201 },
	{ 11, 201 },
	{ 139, 201 },

	// Right-from-down
	{ 72, 200 },
	{ 104, 200 },

	// Turn left
	{ 18, 187 },
	{ 22, 187 },
	{ 150, 187 },
	{ 138, 187 },

	// Left-from-down
	{ 80, 188 },
	{ 208, 188 },

	// T shape right
	{ 74, 204 },

	// T shape left
	{ 82, 185 },

	// T shape down
	{ 26, 203 },

	// T Shape Up
	{ 88, 202 },
};

int GameMap::GetWallChar(vec2 pos)
{
	bool up = GetCollision(pos + moves[M_UP]);
	bool dn = GetCollision(pos + moves[M_DOWN]);
	bool rt = GetCollision(pos + moves[M_RIGHT]);
	bool lt = GetCollision(pos + moves[M_LEFT]);

	// Handling simple cases first
	if (!up && !dn && !rt && !lt) {
		return 205;///'═';
	}

	if ((up || dn) && !rt && !lt) {
		return 186;//'║';
	}

	// Each wall has own bit in 8bit integer
	// We then just lookup premade table to know which wall to place
	// I made this table with external tool, which I made also myself
	// Made a programm to make a program huh
	bool upR = GetCollision(pos + moves[M_UP]   + moves[M_RIGHT]);
	bool dnR = GetCollision(pos + moves[M_DOWN] + moves[M_RIGHT]);
	bool upL = GetCollision(pos + moves[M_UP]   + moves[M_LEFT]);
	bool dnL = GetCollision(pos + moves[M_DOWN] + moves[M_LEFT]);

	int wallKey = upL << 7 | up << 6 | upR << 5 |
				  lt  << 4 |			rt << 3 |
				  dnL << 2 | dn << 1 | dnR;

	auto foundWall = wallMap.find(wallKey);
	if (foundWall != wallMap.end())
		return foundWall->second;

	return 205;  // Just horizontal wall
}

// This method creates a special char for each wall, so we don't need to do this
// every frame (quite expensive)
void GameMap::BakeWalls()
{
	wallCharsMap.clear();

	for (auto pos : walls)
	{
		wallCharsMap.insert_or_assign(pos, GetWallChar(pos));
	}

	wallCharsMap.insert_or_assign(ghostGate, GetWallChar(ghostGate));
}

// True if given position has object of desired type in it
bool GameMap::GetCollision(vec2 pos, int targetCollision)
{
	if (IsOutOfBounds(pos))
		return false;

	switch (targetCollision)
	{
	default:
		return false;
	case COLLISION_WALL:
		return walls.find(pos) != walls.end();
	case COLLISION_DOT:
		return dots.find(pos) != dots.end();
	case COLLISION_ENERGIZER:
		return powerUps.find(pos) != powerUps.end();
	case COLLISION_GHOST_WALL:
		return ghostGate == pos;
	}
}

// Actually draws contents of map to screen
void GameMap::Draw(Screen* screen, vec2 offset)
{
	for (int x = 0; x < mapSizeX; x++)
	{
		for (int y = 0; y < mapSizeY; y++)
		{
			vec2 pos = vec2{ x,y };
			char charToDraw = ' ';
			const char* color = COL_RESET;

			if (walls.find(pos) != walls.end()) {
				charToDraw = wallCharsMap[pos];
				color = COL_BLUE;
			}
			else if (pos == ghostGate) {
				charToDraw = wallCharsMap[pos];
				color = COL_WHITE;
			}
			else if (dots.find(pos) != dots.end()) {
				charToDraw = DOT_CHAR;
			}
			else if (powerUps.find(pos) != powerUps.end()) {
				charToDraw = POWER_UP_CHAR;
				color = COL_BRIGHT_YELLOW;
			}

			screen->SetPixel(pos + offset, charToDraw);
			screen->SetColor(pos + offset, color);
		}
	}
}

int GameMap::GetCount(int objectType)
{
	switch (objectType)
	{
	case COLLISION_DOT:
		return dots.size();

	case COLLISION_ENERGIZER:
		return powerUps.size();
	}

	throw std::invalid_argument("Only COLLISION_DOT and "
					  "COLLISION_ENERGIZER are supported");
}

// Destroys object from the map if it exists
// Returns how many objects remains
void GameMap::Destroy(int objectType, vec2 position)
{
	switch (objectType)
	{
	case COLLISION_DOT:
		dots.erase(position);
		return;

	case COLLISION_ENERGIZER:
		powerUps.erase(position);
		return;
	}

	throw std::invalid_argument("Only COLLISION_DOT and COLLISION_ENERGIZER"
								" are supported");
}

bool GameMap::IsOutOfBounds(vec2 pos)
{
	return pos.x < 0 || pos.x > mapSizeX - 1 || 
		   pos.y < 0 || pos.y > mapSizeY - 1;
}

vec2 GameMap::GetMapSize() {
	return vec2(mapSizeX, mapSizeY);
}

vec2 GameMap::GetGhostGatePos()
{
	return ghostGate;
}

GameMap::~GameMap() {
	delete[] gameMap;
}