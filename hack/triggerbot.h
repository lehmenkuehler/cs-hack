#pragma once
#include <ctime>

class Triggerbot
{
public:
	Triggerbot();
	void Activate(bool spraydown);
private:
	clock_t t_target_killed;
	bool target_alive_last_tick;
	int target_id;
	int real_target_id;
	clock_t t_on_target;
	clock_t t_shot_scheduled;
	bool shot_scheduled;

	float distance;
	void Search();
	void ScheduleShot();
	void Fire();
	bool OnTarget();
	bool OnTargetIncross();
	bool OnTargetHitbox();
};