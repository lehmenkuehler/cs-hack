#include <cmath>

#include "triggerbot.h"
#include "memory.h"
#include "entity.h"
#include "weapon.h"
#include "utility.h"

Triggerbot::Triggerbot()
{
	t_target_killed = 0;
	target_id = -1;
	real_target_id = -1;
	target_alive_last_tick = false;
	t_on_target = std::clock();
	t_shot_scheduled = std::clock();
	shot_scheduled = false;
	distance = 0.f;
}

void Triggerbot::Activate(bool spraydown)
{
	int pause = ptr_active_weapon->GetActiveConfig()->triggerbot_pause_after_shot;
	if (target_id != -1)
	{
		if (target_alive_last_tick && ((ptr_entity_list + target_id)->life_state == LIFE_STATE_DYING || (ptr_entity_list + target_id)->life_state == LIFE_STATE_DEAD))
		{
			t_target_killed = std::clock();
			target_alive_last_tick = false;
		}

		float target_distance = ((ptr_entity_list + target_id)->origin - ptr_local_entity->origin).Abs();
		pause += static_cast<int>(ptr_active_weapon->GetActiveConfig()->triggerbot_pause_range_extension_factor * target_distance);
	}

	if (ptr_active_weapon->GetActiveConfig()->triggerbot_enable_in_scope_only && !ptr_local_entity->in_scope) return;
	if (Utility::ClockDiff(std::clock(), ptr_local_entity->t_last_weapon_change) < ptr_active_weapon->time) return;

	if (!spraydown && Utility::ClockDiff(std::clock(), ptr_local_entity->t_shot_fired) < pause) return;
	if (ptr_local_entity->velocity > ptr_active_weapon->GetActiveConfig()->triggerbot_maximum_velocity) return;

	if (Utility::ClockDiff(std::clock(), t_target_killed) < static_cast<int>(static_cast<float>(ptr_active_weapon->GetActiveConfig()->triggerbot_fake_shot_duration)) &&
		target_id == real_target_id &&
		Utility::ClockDiff(std::clock(), t_on_target) < 1000)
	{
		Fire();
		return;
	}

	Search();
	distance = ((ptr_entity_list + target_id)->origin - ptr_local_entity->origin).Abs();
	if (distance > ptr_active_weapon->GetActiveConfig()->triggerbot_maximum_distance) return;
	if (target_id != -1 && (ptr_entity_list + target_id)->life_state == LIFE_STATE_ALIVE) target_alive_last_tick = true;

	if (ptr_active_weapon->GetActiveConfig()->triggerbot_disable_while_entering_scope && ptr_local_entity->in_scope && Utility::ClockDiff(std::clock(), ptr_local_entity->t_not_in_scope) < 1000)
	{
		float distance_factor = distance / 4000.f;
		if (distance_factor > 1.f) distance_factor = 1.f;
		if (Utility::ClockDiff(std::clock(), ptr_local_entity->t_not_in_scope) < static_cast<int>(100.f + distance_factor * 700.f)) return;
	}

	if (ptr_active_weapon->GetActiveConfig()->triggerbot_head_only)
	{
		float comp_val = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_head).DistanceAbs(ptr_local_entity->local_view).pitch;
		float head_radius = std::atan(4.f / distance) * 180.f / PI;
		if (comp_val > head_radius) return;
	}

	if (OnTarget())
	{
		t_on_target = std::clock();
		ScheduleShot();
	}

	if (spraydown && Utility::ClockDiff(std::clock(), t_on_target) < ptr_active_weapon->GetActiveConfig()->triggerbot_fire_extension) ScheduleShot();

	if (shot_scheduled && Utility::ClockDiff(std::clock(), t_shot_scheduled) >= ptr_active_weapon->GetActiveConfig()->triggerbot_delay) Fire();
}

void Triggerbot::Search()
{
	float diff = 99999.f;
	int tmp_target_id = -1;

	for (int i = 0; i <= 64; i++)
	{
		if ((ptr_entity_list + i) == 0) continue;

		if ((ptr_active_weapon->GetActiveConfig()->triggerbot_friendly_fire || (ptr_entity_list + i)->team != ptr_local_entity->team) && (ptr_entity_list + i)->dormant == 0 && (ptr_entity_list + i)->life_state == LIFE_STATE_ALIVE && !(ptr_entity_list + i)->immune)
		{
			float comp_val;
			comp_val = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + i)->origin_head).DistanceAbs(ptr_local_entity->local_view).Abs();
			if (comp_val < diff)
			{
				tmp_target_id = i;
				diff = comp_val;
			}
		}
	}
	target_id = tmp_target_id;
}

void Triggerbot::ScheduleShot()
{
	if (ptr_active_weapon->GetActiveConfig()->triggerbot_delay == 0) Fire();
	else if (!shot_scheduled)
	{
		shot_scheduled = true;
		t_shot_scheduled = std::clock();
	}
}

// dangerous click spam
void Triggerbot::Fire()
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
	ptr_local_entity->t_shot_fired = std::clock();
	shot_scheduled = false;
}

bool Triggerbot::OnTarget()
{
	switch (ptr_active_weapon->GetActiveConfig()->triggerbot_mode)
	{
	case 0:
		return OnTargetIncross();
	case 1:
		return OnTargetHitbox();
	case 2:
		return OnTargetIncross() || OnTargetHitbox();
	default:
		return false;
	}
}

bool Triggerbot::OnTargetIncross()
{
	if (ptr_local_entity->crosshair_id == 0) return false;
	if (!(ptr_entity_list + ptr_local_entity->crosshair_id)->player) return false;
	if (!ptr_active_weapon->GetActiveConfig()->triggerbot_friendly_fire && ptr_local_entity->team == (ptr_entity_list + ptr_local_entity->crosshair_id)->team) return false;
	real_target_id = ptr_local_entity->crosshair_id;
	return true;
}

bool Triggerbot::OnTargetHitbox()
{
	if (ptr_active_weapon->GetActiveConfig()->triggerbot_visibility_check && !(ptr_entity_list + target_id)->spotted_by_local_entity) return false;
	if (Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_head).DistanceAbs(ptr_local_entity->local_view).Abs() < std::atan(5.f / distance) * 180.f / PI ||
		Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_chest).DistanceAbs(ptr_local_entity->local_view).Abs() < std::atan(8.f / distance) * 180.f / PI ||
		Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_stomach).DistanceAbs(ptr_local_entity->local_view).Abs() < std::atan(8.f / distance) * 180.f / PI)
	{
		real_target_id = ptr_local_entity->crosshair_id;
		return true;
	}
	else return false;
}