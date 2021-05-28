#pragma once
#include "vec2.h"
#include "AActor.h"

class GameSession;
class InputHandler;

class Player : public AActor
{
public:
	Player(vec2 position, GameSession* session, InputHandler* input);
	~Player() = default;

	void Update(int time, int deltaTime) override;
	void Respawn() override;

	void SetSpawnPosition(vec2 p);
	vec2 GetSpawnPosition();

	vec2 GetDirection();

	int GetPowerUpTime();
	void SetPowerUpTime(int ms);

	int GetEatenCount();
	void SetEatenCount(int count);

private:

	void UpdatePlayer();

	vec2 direction;

	int nextPlayerUpdateTime;
	int playerPowerUpTime;
	int eatenDuringPowerUp;

	InputHandler* input;
};

