#include "InputHandler.h"
#include <Windows.h>
#include "GameSettings.h"

const vec2 moves[4]
{
	vec2 {  0,  -1 },  // Up
	vec2 { -1,   0 },  // Left
	vec2 {  0,   1 },  // Down
	vec2 {  1,   0 },  // Right
};

bool InputHandler::isVKPressed(int code)
{
	// If the most significant bit is set, the key is down, 
	// and if the least significant bit is set, 
	// the key was pressed after the previous call to 
	// GetAsyncKeyState.
	// However, you should not rely on this last behavior; 

	// GetAsyncKeyState returns short
	// Which can count up to 2^15
	// this least sagnificant is 2^15
	return GetAsyncKeyState(code) & (1 << 15);
}

// После парочки тестов, все мои знакомые заявили, 
// что попадать в узкие места почему то трудно. 
// Сначала я подумал, что дело в слишком высокой скорости игрока,
// но дело оказалось
// вовсе не в ней. 
// inputVec считывался только в момент, 
// когда игрок практически сдвинулся
// то есть для моментального попадания нужно было 
// уложиться в ~10мс обновления между кадрами
// Поэтому вместо этого мы накапливаем весь ввод, 
// а когда игрок хочет подвинуться и получает
// inputVec - мы его обнуляем
int InputHandler::GetInputVec() 
{
	return inputVec;
}

void InputHandler::ReleaseInputVec() {
	inputVec = 0;
}

void InputHandler::Update() 
{
	// Space handle
	{
		bool vkSpacePressed = isVKPressed(VK_SPACE);
		bool pressedSpace = false;
		if (vkSpacePressed && !pressedSpaceBefore)
		{
			pressedSpace = true;
			pressedSpaceBefore = true;
		}
		else if (!vkSpacePressed && pressedSpaceBefore) {
			pressedSpaceBefore = false;
		}

		space = pressedSpace;
	}

	// Escape handle
	{
		bool vkEscapePressed = isVKPressed(VK_ESCAPE);
		bool pressedEscape = false;
		if (vkEscapePressed && !pressedEscBefore)
		{
			pressedEscape = true;
			pressedEscBefore = true;
		}
		else if (!vkEscapePressed && pressedEscBefore) {
			pressedEscBefore = false;
		}

		esc = pressedEscape;
	}
	
	// F6 handle
	{
		bool vkPressed = isVKPressed(VK_F6);
		bool pressed = false;
		if (vkPressed && !pressedF6Before)
		{
			pressed = true;
			pressedF6Before = true;
		}
		else if (!vkPressed && pressedF6Before) {
			pressedF6Before = false;
		}

		f6 = pressed;
	}

	// These are powers of 2
	// I could also change I_KEY to (1<<M_KEY)
	inputVec |= (1 << M_UP   ) * isVKPressed(VK_UP)
			 |  (1 << M_DOWN ) * isVKPressed(VK_DOWN)
			 |  (1 << M_RIGHT) * isVKPressed(VK_RIGHT)
			 |  (1 << M_LEFT ) * isVKPressed(VK_LEFT);
}

bool InputHandler::IsSpacePressed()
{
	return space;
}

bool InputHandler::IsEscapePressed() 
{
	return esc;
}

bool InputHandler::IsF6Pressed()
{
	return f6;
}