#pragma once
#include <string>

using std::string;

enum Color
{
	BLACK = 0,
	DARK_BLUE = 1,
	DARK_GREEN = 2,
	DARK_CYAN = 3,
	DARK_RED = 4,
	DARK_PINK = 5,
	DARK_YELLOW = 6,
	MEDIUM_GRAY = 7,
	GRAY = 8,
	BLUE = 9,
	GREEN = 10,
	CYAN = 11,
	RED = 12,
	PINK = 13,
	YELLOW = 14,
	WHITE = 15
};

namespace Output
{
	void SetRandomConsoleTitle(int length);
	void ClearConsole();
	void PrintToConsole(string message, Color color_text, Color color_bg);
	void PrintToConsole(short lb1, string message, Color color_text, Color color_bg);
	void PrintToConsole(string message, short lb2, Color color_text, Color color_bg);
	void PrintToConsole(short lb1, string message, short lb2, Color color_text, Color color_bg);
	void PrintErrorAndExit(string message, unsigned short time_exit);
	void PrintTime();
	string GenerateRandomString(const int len);
};