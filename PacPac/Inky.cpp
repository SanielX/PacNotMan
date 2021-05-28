#include "Inky.h"
#include "GameSettings.h"
Inky::Inky(GameSession* mySession, vec2 spawnPoint)
	: AGhost(mySession, spawnPoint)
{}

Inky::~Inky() {}

int Inky::GetTickRate() {
	return INKY_SPEED;
}

GhostName Inky::GetGhostName() {
	return GhostName::NAME_Inky;
}

const char* Inky::GetGhostColor() const {
	return COL_BRIGHT_CYAN;
}

// Inky runs in the places Blinky doesn't
vec2 Inky::SelectTarget()
{
	vec2 target = mySession->GetPlayerPosition();
	vec2 playerDir = mySession->GetPlayerDirection();
	target = target + playerDir;

	if(playerDir.y > 0)
	{
		// Making that old bug work. Again
		target = target + vec2(-1, 1); 
	}

	vec2 pinky = mySession->GetGhostPosition(GhostName::NAME_Pinky);
	// vector from player to pinky rotated 180 degrees
	vec2 pinky2Player = target - pinky;  

	return pinky2Player * 2;
}

vec2 Inky::SelectScatterTarget()
{
	vec2 levelSize = mySession->GetMap()->GetMapSize();
	return vec2{ levelSize.x - 3, levelSize.y + 3 };
}