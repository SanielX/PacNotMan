#pragma once
#include "vec2.h"
#include "GameSession.h"
#include "AActor.h"

enum GhostState 
{
	// У цьому стані привид завжди 
	// намагається рухатись уверх
	Spawned,  
	Chase,
	Scatter,
	Flee,  // Наляканий стан
	Eaten 
};

// Ghost names are used to lookup other ghosts
// Since Inky's AI is based around not only player's, 
// but Blinky's position as well
enum GhostName 
{
	NAME_Blinky,
	NAME_Pinky,
	NAME_Inky,
	NAME_Clyde
};

// Абстрактний клас для всіх інших привидів
// Визначає спільну поведінку для кожного з них
// Додає абстрактні методи, потрібні для задання
// унікальної поведінки кожному привиду
class AGhost : public AActor
{
public:
	AGhost(GameSession* mySession, vec2 spawnPoint);
	virtual ~AGhost() = 0;

	void Update(int time, int deltaTime) sealed override;
	// Teleport to original spawn position
	void Respawn() sealed override;

	// Turn ghost in a frightened mod 
	// for the number of seconds
	void Frighten(int time);
	// Eat ghost so it returns to "ghost house" 
	// (which is a place below ghost wall)
	void Eat();
	void Sleep(int ms);
	// Returns value from GhostState enum
	int GetState();

	static int GhostSleepTime(int name, int lvl);

	virtual int GetTickRate() = 0;
	virtual GhostName GetGhostName() = 0;
	virtual const char* GetGhostColor() const = 0;
protected:
	virtual vec2 SelectTarget() = 0;
	virtual vec2 SelectScatterTarget() = 0;

	bool IsMovePossible(vec2 move, bool ignoreGhostWalls);

private:
	int CalcTick();

	void Move(vec2 direction);
	void MoveFromSpawn();
	void EnterScatter();
	void EnterChase();
	vec2 PickDirection(vec2 targetTile);

	vec2 lastMove;   // To disallow moving backwards

	int lastUpdateTime;
	int stateTimer;
	int scatterTimes;
	GhostState myState;
};

