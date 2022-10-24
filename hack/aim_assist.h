#pragma once
#include <ctime>

class AimAssist
{
public:
	AimAssist();
	void Activate();
	void Reset();
private:
	std::clock_t t_activation;
	std::clock_t t_start;
	std::clock_t t_last_tick;
	std::clock_t t_last_velocity_update;
	std::clock_t t_target_killed;

	bool active_last_tick;
	float distance;
	int target_id;
	short target_bone;

	void ConvergeViewAngle();
	float HitboxRadius();
	bool Search(int* id);
};