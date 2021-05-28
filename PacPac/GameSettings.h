#pragma once
enum GhostName;

// General ghost. Imagine there is "When" before each
// to understand
extern const int EATEN_SPEED;
extern const int FRIGHTENED_SPEED;
extern const int FRIGHTNED_TIME;   // Час, на який привид становиться
								   // наляканим після того як Пакман 
								   // з'їсть енерджайзер

// Blinky 
extern const int BLINKY_SPEED;
extern const int BLINKY_SPEEDUP_DOTS;  // К-сть точок, нижче якої
									   // Блінкі підвищує швидкість
extern const int BLINKY_ANGRY_SPEED;

// Others
extern const int INKY_SPEED;
extern const int PINKY_SPEED;
extern const int CLYDE_SPEED;

// Scores
extern const int DOT_SCORE;
extern const int ENERGIZER_SCORE;

// Ghost AI settings

constexpr int INF_STATE = -255;  // Позначає значення таймеру у привидів
								 // за якого вони перестають змінювати
								 // режимими погоні та розсіювання
constexpr int SCATTERS_COUNT = 5;
constexpr int SCATTERS_PER_LEVEL_COUNT = 5;

// Level to scatter index
extern const int SCATTER_TIMINGS[SCATTERS_COUNT][SCATTERS_PER_LEVEL_COUNT];

constexpr int CHASES_COUNT = 2;
constexpr int CHASES_PER_LEVEL_COUNT = 4;
// Level to chase index
extern const int CHASE_TIMINGS[CHASES_COUNT][CHASES_PER_LEVEL_COUNT];

// Player
extern const int DEFAULT_LIVES;
extern const int PLAYER_SPEED;