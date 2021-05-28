#pragma once
#include <stdexcept>

class user_exception
{
public:
	user_exception(const char* header, const char* info);
	user_exception(const char* info);
	~user_exception();

	void ShowError();

private:
	char* header;
	char* info;
};