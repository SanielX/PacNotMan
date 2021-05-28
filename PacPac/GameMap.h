#pragma once
#include "vec2.h"
#include "Screen.h"
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

using std::vector;
using std::set;
using std::map;

enum CollisionType 
{
	COLLISION_WALL,
	COLLISION_GHOST_WALL,
	COLLISION_DOT,
	COLLISION_ENERGIZER
};

class GameSession;
class AGhost;

bool CompareText(const char* txt0, const char* txt1);
char* CopyText(const char* txt);

// Клас, який зчитує текст мапи та перетворює на
// ігрові об'єкти. Зберігає у собі статичні об'єкти
// та надає інтерфейс для вазємодії з ними
class GameMap
{
public:
	GameMap(const char* map);
	~GameMap();

	void Setup(int lvl, GameSession* mySession,
			   vec2& player, vector<AGhost*>& ghosts);
	bool GetCollision(vec2 pos, int targetCollision = COLLISION_WALL);
	vec2 GetMapSize();
	void Draw(Screen* screen, vec2 offset);
	// Only dots and energizers are allowed.
	// Returns count of objects left
	void Destroy(int objectType, vec2 pos);
	int GetCount(int objectType);
	// Ghost gate is displayed as white wall on the map
	// It is represented by char W and means that a pixel below it
	// is a spawnpoint of ghosts and a place where they should return
	// after being eaten
	vec2 GetGhostGatePos();

private:
	int GetWallChar(vec2 pos);
	void BakeWalls();
	bool IsOutOfBounds(vec2 pos);

	int mapSizeX, mapSizeY;
    char* gameMap;

	map<vec2, int> wallCharsMap;
	set<vec2> walls, dots, powerUps;
	vec2 ghostGate;

	static std::unordered_map<int, char> wallMap;
};

