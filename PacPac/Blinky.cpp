#include "Blinky.h"
#include "GameSettings.h"

Blinky::Blinky(GameSession* mySession, vec2 spawnPoint)
	: AGhost(mySession, spawnPoint)
{}

Blinky::~Blinky() {}

int Blinky::GetTickRate()
{
	if (mySession->DotsLeft() < BLINKY_SPEEDUP_DOTS)
	{
		return  BLINKY_ANGRY_SPEED;
	}
	else
	{
		return  BLINKY_SPEED;
	}
}

GhostName Blinky::GetGhostName() {
	return GhostName::NAME_Blinky;
}

const char* Blinky::GetGhostColor() const {
	return COL_RED;
}

// Blinky just hates you
vec2 Blinky::SelectTarget() {
	return mySession->GetPlayerPosition();
}

vec2 Blinky::SelectScatterTarget()
{
	// Blinky chases pacman even in flee state 
	// if there are almost no dots left
	if (mySession->DotsLeft() < BLINKY_SPEEDUP_DOTS)
	{
		return SelectTarget();
	}
	else
	{
		vec2 levelSize = mySession->GetMap()->GetMapSize();
		return vec2{ levelSize.x, -5 };
	}
}