#include <bitset>

#include "aimbot.h"
#include "memory.h"
#include "entity.h"
#include "configuration.h"
#include "weapon.h"
#include "utility.h"

Aimbot::Aimbot()
{
	shot_fired = false;
	active_last_tick = false;
}

void Aimbot::Update()
{
	if (ptr_local_entity->ammunition == 0) return;
	if (ptr_local_entity->reloading) return;
	if (Utility::ClockDiff(std::clock(), ptr_local_entity->t_last_weapon_change) < ptr_active_weapon->time) return;

	if (!active_last_tick || Utility::ClockDiff(std::clock(), ptr_local_entity->t_last_weapon_change) < 50) shot_fired = false;
	active_last_tick = true;

	if (ptr_active_weapon->GetActiveConfig()->aimbot_single_shot && shot_fired) return;

	int id;
	if (Search(&id))
	{
		Memory::SetAngle(Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + id)->origin_head).SubtractNorm(ptr_local_entity->local_punch.Multiply(2.f)));

		if (ptr_active_weapon->GetActiveConfig()->aimbot_single_shot)
		{
			// dangerous
			INPUT input = { 0 };
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &input, sizeof(INPUT));
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &input, sizeof(INPUT));
			shot_fired = true;
			ptr_local_entity->t_shot_fired = std::clock();
		}
	}
	else if (ptr_active_weapon->GetActiveConfig()->aimbot_fire_if_no_target)
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
		shot_fired = true;
		ptr_local_entity->t_shot_fired = std::clock();
	}
}

void Aimbot::SetInactive()
{
	active_last_tick = false;
}

bool Aimbot::Search(int* id)
{
	float diff = 99999.f;
	int target_id = -1;

	for (int i = 0; i <= 256; i++)
	{
		if ((ptr_entity_list + i) == 0) continue;

		if ((ptr_active_weapon->GetActiveConfig()->aimbot_friendly_fire || (ptr_entity_list + i)->team != ptr_local_entity->team) && (ptr_entity_list + i)->dormant == 0 && (ptr_entity_list + i)->life_state == LIFE_STATE_ALIVE && !(ptr_entity_list + i)->immune)
		{
			if (ptr_active_weapon->GetActiveConfig()->aimbot_visibility_check && !(ptr_entity_list + i)->spotted_by_local_entity) continue;

			float comp_val;

			comp_val = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + i)->origin_head).DistanceAbs(ptr_local_entity->local_view).Abs();
			if (comp_val < diff && comp_val < ptr_active_weapon->GetActiveConfig()->aimbot_field_of_view / 2.f)
			{
				target_id = i;
				diff = comp_val;
			}
		}
	}

	if (target_id == -1)	return false;
	else
	{
		*id = target_id;
		return true;
	}
}