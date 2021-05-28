#pragma once
#include "vec2.h"

class GameSession;

// Абстрактний клас шаблон для динамічних об'єктів
class AActor
{
public:
	virtual ~AActor() = 0;
	AActor(GameSession* session);

	virtual void Update(int time, int deltaTime) = 0;
	virtual void Respawn() = 0;

	vec2 GetPosition();

protected:
	vec2 position;
	vec2 spawnPoint;
	GameSession* mySession;
};

