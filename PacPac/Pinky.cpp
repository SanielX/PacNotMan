#include "Pinky.h"
#include "GameSettings.h"

Pinky::Pinky(GameSession* mySession, vec2 spawnPoint)
	: AGhost(mySession, spawnPoint)
{}

Pinky::~Pinky() {}

int Pinky::GetTickRate() {
	return  PINKY_SPEED;
}

GhostName Pinky::GetGhostName() {
	return GhostName::NAME_Pinky;
}

const char* Pinky::GetGhostColor() const {
	return COL_BRIGHT_MAGNETA;
}

// Pinky tries to outrun you from front
vec2 Pinky::SelectTarget()
{
	vec2 target = mySession->GetPlayerPosition();
	vec2 playerDir = mySession->GetPlayerDirection();
	if (playerDir.y > 0)
	{
		target = target + (playerDir * 3);
	}
	else
	{
		target = target + vec2{ -2, 3 }; // Making that old bug work
	}
	return target;
}

vec2 Pinky::SelectScatterTarget()
{
	return vec2{ 3, -3 };
}