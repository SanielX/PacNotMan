#pragma once

// ���������, ��� ������ � ��� ������
// ansi escape code �������

struct AsciiColor
{
	AsciiColor();
	AsciiColor(const AsciiColor& other);
	AsciiColor(const char* col);
	~AsciiColor();

	void operator=(const AsciiColor& other);
	bool operator==(const AsciiColor& other);
	bool operator!=(const AsciiColor& other);

	const char* colorPtr;
};

