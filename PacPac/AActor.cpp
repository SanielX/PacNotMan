#include "AActor.h"
#include "vec2.h"
#include "GameSession.h"
#include <stdexcept>

AActor::~AActor(){}

AActor::AActor(GameSession* session) 
{
	mySession = session;
}

vec2 AActor::GetPosition() 
{
	return position;
}

