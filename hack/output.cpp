#include <iomanip>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <algorithm>
#include <random>

#include "output.h"

using std::cout;
using std::cin;
using std::endl;

void Output::SetRandomConsoleTitle(int length)
{
	SetConsoleTitle(GenerateRandomString(length).c_str());
}

void Output::ClearConsole()
{
	COORD top_left = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, top_left, &written);
	FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, top_left, &written);
	SetConsoleCursorPosition(console, top_left);
}

void Output::PrintToConsole(string message, Color color_text, Color color_bg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (static_cast<WORD>(color_text) + static_cast<WORD>(16) * static_cast<WORD>(color_bg)));
	std::cout << message;
}

void Output::PrintToConsole(short lb1, string message, Color color_text, Color color_background)
{
	while (lb1 > 0)
	{
		cout << endl;
		lb1--;
	}
	PrintToConsole(message, color_text, color_background);
}

void Output::PrintToConsole(string message, short lb2, Color color_text, Color color_background)
{
	PrintToConsole(message, color_text, color_background);
	while (lb2 > 0)
	{
		cout << endl;
		lb2--;
	}
}

void Output::PrintToConsole(short lb1, string message, short lb2, Color color_text, Color color_background)
{
	while (lb1 > 0)
	{
		cout << endl;
		lb1--;
	}
	PrintToConsole(message, color_text, color_background);
	while (lb2 > 0)
	{
		cout << endl;
		lb2--;
	}
}

void Output::PrintErrorAndExit(string message, unsigned short time_to_exit)
{
	ClearConsole();
	PrintToConsole(message, 1, RED, BLACK);
	Sleep(time_to_exit * 1000);
	exit(0);
}

#pragma warning( disable : 4505)
void Output::PrintTime()
{
	auto t = std::time(nullptr);
#pragma warning(disable : 4996)
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S");
	auto time = oss.str();
	PrintToConsole("[" + time + "] ", WHITE, BLACK);
}

string Output::GenerateRandomString(int length)
{
	static auto& chrs = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	thread_local static std::mt19937 rg{ std::random_device{}() };
	thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);
	std::string s;
	s.reserve(length);
	while (length--) s += chrs[pick(rg)];
	return s;
}