#pragma once
#include "AGhost.h"

class Inky : public AGhost
{
public:
    Inky(GameSession* mySession, vec2 spawnPoint);
    ~Inky();

    int GetTickRate();
    GhostName GetGhostName();
    const char* GetGhostColor() const;

protected:
    vec2 SelectTarget();
    vec2 SelectScatterTarget();
};
