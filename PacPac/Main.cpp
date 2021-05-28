// Курсова робота за темою
// Клон PacMan у консолі Windows
// Автор
// Студент групи КНТ 20-2
// Ходика Олександр Сергійович

#include "PacNotMan.h"
#include <Windows.h>
#include <string.h>
using namespace std;

int main()
{
	std::srand(time(0));
	// Раземенування у ссилку не створює копію
	PacNotMan& game = *(new PacNotMan());

	bool exit = false;
	chrono::steady_clock clock;

	unsigned ms = 0;
	while (!exit) 
	{  
		auto n0 = clock.now();
		exit = game(ms);
		auto n1 = clock.now();
		ms = chrono::duration_cast<chrono::milliseconds>(n1 - n0).count();
	}

	delete &game;
	return 0;
}