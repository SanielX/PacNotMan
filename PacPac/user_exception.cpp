#include "user_exception.h"
#include <stdexcept>
#include "GameMap.h"
#include <Windows.h>

user_exception::user_exception(const char* info) 
	: user_exception("Error", info) { }

user_exception::user_exception(const char* header, const char* info) 
{
	this->header = CopyText(header);
	this->info = CopyText(info);
}

user_exception::~user_exception() 
{
	delete[] info;
	delete[] header;
}

void user_exception::ShowError() 
{
	MessageBoxA(NULL, info, header, MB_ICONERROR);
}