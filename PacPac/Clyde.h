#pragma once
#include "AGhost.h"

// Clyde tries to catch you
// But when too close - runs away
class Clyde : public AGhost
{
public:
    Clyde(GameSession* mySession, vec2 spawnPoint);
    ~Clyde();

    int GetTickRate();
    GhostName GetGhostName();
    const char* GetGhostColor() const;

protected:
    vec2 SelectTarget();
    vec2 SelectScatterTarget();
};

