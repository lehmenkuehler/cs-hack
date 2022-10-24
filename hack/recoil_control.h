#pragma once
#include <ctime>

#include "entity.h"
#include "configuration.h"
#include "axis.h"

class RecoilControl
{
public:
	RecoilControl();
	void Activate();
	void Reset();
private:
	Axis punch_backup;
	float tracking_value_yaw, tracking_value_pitch;
	std::clock_t t_last_compensation;
	std::clock_t t_random_pitch, t_random_yaw;
};