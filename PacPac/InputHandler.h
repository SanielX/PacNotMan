#pragma once
#include "vec2.h"

// Move priority:
// Up, Left, Down, Right
enum Move
{
	M_UP,
	M_LEFT,
	M_DOWN,
	M_RIGHT
};

extern const vec2 moves[4];

class InputHandler
{
public:
	void Update();

	// Input vector is a packed integer with 4 meaningful bits
	// Going in move priority UP LEFT DOWN RIGHT
	// We could return vec2 but player movement has priorities
	// And also up+down is 0, but actually player should move where
	// available
	int GetInputVec();
	// Sets inputVec to 0, call when input has been used
	void ReleaseInputVec();

	// Next functions are true only when button state changed
	// from unpressed to pressed between checks
	bool IsSpacePressed();
	bool IsEscapePressed();
	bool IsF6Pressed();  // Used to "save" the game

private:
	bool isVKPressed(int code);

	int inputVec = 0;
	bool esc, space, f6;
	
	bool pressedF6Before = false;
	bool pressedEscBefore = false;
	bool pressedSpaceBefore = false;
};

