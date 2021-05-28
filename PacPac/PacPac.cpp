#include "PacNotMan.h"
using namespace std;

int main()
{
	std::srand(time(0));
	PacNotMan* game = new PacNotMan();

	bool exit = false;
	while (!exit) 
	{
		exit = game->Update(10);

		chrono::milliseconds timespan(10); // or whatever
		this_thread::sleep_for(timespan);
	}
}