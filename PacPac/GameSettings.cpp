#include "GameSettings.h"
#include "AGhost.h"

// Speed is actually expressed in 1/speed, because it's a tickrate
// Anwyay, lower value - higher speed

// General ghost
const int EATEN_SPEED = 50 * 2;
const int FRIGHTENED_SPEED = 800;
const int FRIGHTNED_TIME = 6000;

// Blinky 
const int BLINKY_SPEEDUP_DOTS = 20;
const int BLINKY_SPEED = 200 * 2;
const int BLINKY_ANGRY_SPEED = 150 * 2;

// Others
const int INKY_SPEED = 270 * 2;
const int PINKY_SPEED = 275 * 2;
const int CLYDE_SPEED = 265 * 2;

// Scores
const int DOT_SCORE = 10;
const int ENERGIZER_SCORE = 50;

// Level - scatter index
const int SCATTER_TIMINGS[SCATTERS_COUNT][SCATTERS_PER_LEVEL_COUNT]
{
	{ 2500, 2500, 1000, 1000, 10 },
	{ 2500, 2500, 1000, 10, 10 },
	{ 2500, 2500, 1000, 10, 10 },
	{ 2500, 2500, 1000, 10, 10 },
	{ 1000, 1000, 1000, 10, 10 },
};

// Level - chase index
const int CHASE_TIMINGS[CHASES_COUNT][CHASES_PER_LEVEL_COUNT]
{
	{ 15000, 15000, 15000, INF_STATE },
	{ 15000, 15000, 7500,  INF_STATE },
};

// Player
const int DEFAULT_LIVES = 3;
const int PLAYER_SPEED = 100 * 2;