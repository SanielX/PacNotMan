#pragma once
#include "AGhost.h"

class Pinky :
    public AGhost
{
public:
    Pinky(GameSession* mySession, vec2 spawnPoint);
    ~Pinky();

    int GetTickRate();
    GhostName GetGhostName();
    const char* GetGhostColor() const;

protected:
    vec2 SelectTarget();
    vec2 SelectScatterTarget();
};

