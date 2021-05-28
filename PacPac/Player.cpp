#include "Player.h"
#include "vec2.h"
#include "GameSettings.h"
#include "GameSession.h"
#include "InputHandler.h"
#include <stdexcept>

using namespace std;

Player::Player(vec2 pos, GameSession* session, InputHandler* handler) 
	: AActor(session)
{
	spawnPoint = pos;
	position = pos;
	direction = vec2(-1, 0);

	input = handler;

	nextPlayerUpdateTime = 0;
	playerPowerUpTime = 0;
	eatenDuringPowerUp = 0;
}

void Player::SetSpawnPosition(vec2 p) 
{
	spawnPoint = p;
}

void Player::Respawn()
{
	position = spawnPoint;
	direction = vec2(-1, 0); // Just so AI behaves good
	nextPlayerUpdateTime =  PLAYER_SPEED;
	eatenDuringPowerUp = 0;
	playerPowerUpTime = 0;
}

void Player::Update(int time, int deltaTime) 
{
	if (playerPowerUpTime > 0)
	{
		playerPowerUpTime -= deltaTime;
		if (playerPowerUpTime < 0)
			playerPowerUpTime = 0;
	}

	if (time > nextPlayerUpdateTime)
	{
		UpdatePlayer();
		nextPlayerUpdateTime = time +  PLAYER_SPEED;
	}
}

void Player::UpdatePlayer()
{
	// In pacman you can run from left 
	// tunnel to right tunnel
	int inputVec = input->GetInputVec();
	input->ReleaseInputVec();
	vec2 newPos = position;

	if (inputVec != 0)
	{
		int inVec = inputVec;
		inputVec = 0;
		for (int i = 0; i < 4; i++)
		{
			int move = inVec & (1 << i);  // Masking
			if (move == 0)
				continue;

			vec2 dir = moves[i];
			vec2 check = position + dir;
			mySession->WrapPos(check);

			if (!mySession->GetMap()->GetCollision(check) &&
				!mySession->GetMap()->GetCollision(check, 
									COLLISION_GHOST_WALL))
			{
				newPos = check;
				direction = dir;
				// Just setting flag so player won't move 
				// to same direction
				inputVec = 1; 
				break;
			}
		}
	}

	// If no input or given input doesn't allow us to move
	// we continue moving where player was moving before
	if (inputVec == 0)
	{
		vec2 check = position + direction;
		mySession->WrapPos(check);
		if (!mySession->GetMap()->GetCollision(check) && 
			!mySession->GetMap()->GetCollision(check, 
								  COLLISION_GHOST_WALL))
			newPos = check;
	}

	position = newPos;
}

vec2 Player::GetSpawnPosition() {
	return spawnPoint;
}

vec2 Player::GetDirection() {
	return direction;
}

int Player::GetPowerUpTime() {
	return playerPowerUpTime;
}

void Player::SetPowerUpTime(int ms) {
	playerPowerUpTime = ms;
}

int Player::GetEatenCount() {
	return eatenDuringPowerUp;
}

void Player::SetEatenCount(int c) {
	eatenDuringPowerUp = c;
}