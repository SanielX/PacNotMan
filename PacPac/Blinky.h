#pragma once
#include "AGhost.h"

// Blinky is a ghost that uses player position as target
// And also if only few dots remains, blinky stops using
// scatter phase

class Blinky :
    public AGhost
{
public:
    Blinky(GameSession* mySession, vec2 spawnPoint);
    ~Blinky();

    int GetTickRate();
    GhostName GetGhostName();
    const char* GetGhostColor() const;

protected:
    vec2 SelectTarget() override;
    vec2 SelectScatterTarget() override;
};

