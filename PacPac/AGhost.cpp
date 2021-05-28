#include <math.h>
#include "AGhost.h"
#include "GameSession.h"
#include "vec2.h"
#include "GameSettings.h"
#include "InputHandler.h"

AGhost::AGhost(GameSession* session, vec2 spawnPoint)
	: AActor(session)
{
	myState = GhostState::Spawned; // It's okay
	lastUpdateTime = 0;
	this->spawnPoint = spawnPoint;
	this->position = spawnPoint;
	lastMove = vec2(0, 0);
}

AGhost::~AGhost() {}

#undef max // wtf
#undef min
using std::min;
using std::max;

int AGhost::GhostSleepTime(int name, int level)
{
	switch (name)
	{
	case NAME_Blinky:
		return max(0, 600 - (100 * level));
	case NAME_Pinky:
		return max(0, 1500 - (100 * level));
	case NAME_Inky:
		return max(0, 4000 - (100 * level));
	case NAME_Clyde:
		return max(0, 7000 - (100 * level));
	}

	return 0;
}

bool AGhost::IsMovePossible(vec2 move, bool ignoreGhostWalls)
{
	auto map = mySession->GetMap();
	bool isWall = map->GetCollision(position + move) ||
			(!ignoreGhostWalls && 
				map->GetCollision(position + move, 
								  COLLISION_GHOST_WALL));

	bool movingBackwards = move == (lastMove * -1);
	return !movingBackwards && !isWall;
}

void AGhost::Eat()
{
	// Змінює напрямок руху на 180
	lastMove = lastMove * -1;
	myState = GhostState::Eaten;
}

int AGhost::GetState()
{
	return myState;
}

// Повертає тікрейт привиду залежно
// від його стану
int AGhost::CalcTick()
{
	switch (myState) {
	default:
		return GetTickRate();
	case GhostState::Flee:
		return  FRIGHTENED_SPEED;
	case GhostState::Eaten:
		return  EATEN_SPEED;
	}
}

// Робить привида неактивним на заданий проміжок часу
void AGhost::Sleep(int ms)
{
	// HACK RIGHT HERE LADIES AND GENTLEMANS
	lastUpdateTime += ms; 
}

void AGhost::Update(int time, int d)
{
	auto deltaTime = time - lastUpdateTime;
	if (time < lastUpdateTime + CalcTick()) {
		return;
	}

	lastUpdateTime = time;

	if (myState == GhostState::Spawned)
	{
		MoveFromSpawn();
		return;
	}

	// Select tile to move to
	vec2 targetTile { 0, 0 };
	switch (myState)
	{
	case GhostState::Eaten:
	{
		// This pushes ghost into their house, 
		// even if ghost spawned somewhere else
		vec2 tar;
		tar = mySession->GetMap()->GetGhostGatePos() + moves[M_DOWN];
		if (position == tar) {
			myState = GhostState::Spawned;  // Change State to chase
			return;
		}

		targetTile = tar;
	}
	break;

	case GhostState::Chase:
		targetTile = SelectTarget();

		if (stateTimer != INF_STATE)
		{
			stateTimer -= deltaTime;
			if (stateTimer <= 0)
				EnterScatter();
		}
		break;

	case GhostState::Scatter:
		targetTile = SelectScatterTarget();
		stateTimer -= deltaTime;
		if (stateTimer <= 0)
		{
			EnterChase();
		}
		break;

	case GhostState::Flee:
		int dir = rand() % 4;
		vec2 moveDir = moves[dir];
		targetTile = position + moveDir;

		stateTimer -= deltaTime;
		if (stateTimer <= 0)
		{
			myState = GhostState::Chase;
		}
		break;
	}

	Move(PickDirection(targetTile));
}

void AGhost::Respawn()
{
	position = spawnPoint;
	myState = GhostState::Spawned;
	scatterTimes = 0;
	stateTimer = 0;
	lastUpdateTime = 50;
}

void AGhost::Frighten(int mseconds)
{
	// Ignore player trying to eat as twice :(
	if (myState == GhostState::Eaten || 
		myState == GhostState::Spawned)
		return;

	myState = GhostState::Flee;
	stateTimer = mseconds;
	lastMove = lastMove * -1;
}

void AGhost::EnterScatter()
{
	myState = GhostState::Scatter;
	int lvlIndex = min(mySession->GetLevel(), SCATTERS_COUNT - 1);
	int index = min(scatterTimes, SCATTERS_PER_LEVEL_COUNT - 1);
	stateTimer =  SCATTER_TIMINGS[lvlIndex][index];
	scatterTimes++;
}

void AGhost::EnterChase()
{
	myState = GhostState::Chase;
	int lvlIndex = min(mySession->GetLevel(), CHASES_COUNT - 1);
	int index = min(scatterTimes, CHASES_PER_LEVEL_COUNT - 1);
	stateTimer =  CHASE_TIMINGS[lvlIndex][index];
}

void AGhost::MoveFromSpawn()
{
	auto map = mySession->GetMap();
	//bool isInSpawn = mySession->IsGhostSpawnPoint(position);
	bool canMoveUp = !map->GetCollision(position + moves[M_UP]);

	if (canMoveUp)
	{
		Move(moves[M_UP]);
		EnterScatter();
	}
	else
	{
		bool canGoLeft = !map->GetCollision(position + moves[M_LEFT]);
		if (canGoLeft)
			Move(moves[M_LEFT]);
		else if (!map->GetCollision(position + moves[M_RIGHT]))
			Move(moves[M_RIGHT]);
	}
}

vec2 AGhost::PickDirection(vec2 targetTile)
{
	// Select Direction
	bool isMovePossibleList[4];
	int distances[4];
	int minDistance = INT_MAX;

	// Discard invalid tiles
	for (size_t i = 0; i < 4; i++)
	{
		isMovePossibleList[i] = IsMovePossible(moves[i], 
										   myState == GhostState::Eaten);

		distances[i] = vec2::SqrDistance(position + moves[i], targetTile);
		if (isMovePossibleList[i] && distances[i] < minDistance)
			minDistance = distances[i];
	}

	// Discard invalid tiles by distance
	for (size_t i = 0; i < 4; i++)
	{
		if (distances[i] > minDistance)
			isMovePossibleList[i] = false;
	}

	// Choosing first available based on priority
	vec2 finalMoveDirection{};
	for (size_t i = 0; i < 4; i++)
	{
		if (isMovePossibleList[i]) {  // Selecting first, by priority
			finalMoveDirection = moves[i];
			break;
		}
	}

	return finalMoveDirection;
}

void AGhost::Move(vec2 dir)
{
	lastMove = dir;
	position = position + dir;

	auto mapSize = mySession->GetMap()->GetMapSize();
	if (position.x < 0)
		position.x = mapSize.x - 1;
	else if (position.x > mapSize.x - 1)
		position.x = 0;
}
