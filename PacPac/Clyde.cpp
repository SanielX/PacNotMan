#include "Clyde.h"
#include "GameSettings.h"
#include "vec2.h"

Clyde::Clyde(GameSession* mySession, vec2 spawnPoint)
	: AGhost(mySession, spawnPoint)
{}

Clyde::~Clyde() {}

int Clyde::GetTickRate() {
	return  CLYDE_SPEED;
}

GhostName Clyde::GetGhostName() {
	return GhostName::NAME_Clyde;
}

const char* Clyde::GetGhostColor() const {
	return COL_ORANGE;
}

// Clyde runs away from pacman when he is too close
vec2 Clyde::SelectTarget()
{
	auto playerPos = mySession->GetPlayerPosition();
	int distToPacman = vec2::SqrDistance(GetPosition(), playerPos);

	if (distToPacman < 6) // In oroginal it's 8, but my map is smaller
		return SelectScatterTarget();
	else
		return playerPos;
}

vec2 Clyde::SelectScatterTarget()
{
	vec2 levelSize = mySession->GetMap()->GetMapSize();
	return vec2{ 2, levelSize.y + 3 };
}