#pragma once
#include <ctime>

class Aimbot
{
public:
	Aimbot();
	void Update();
	void SetInactive();
private:
	bool active_last_tick;
	bool Search(int* id);
	bool shot_fired;
};