#include "recoil_control.h"
#include "axis.h"
#include "weapon.h"
#include "utility.h"

RecoilControl::RecoilControl()
{
	punch_backup = Axis();
	tracking_value_yaw = 1.f;
	tracking_value_pitch = 1.f;
	t_last_compensation = 0;
	t_random_pitch = std::clock();
	t_random_yaw = std::clock();
}

void RecoilControl::Activate()
{
	if (ptr_active_weapon->weapon_type == WeaponType::SNIPER_RIFLE || ptr_active_weapon->weapon_type == WeaponType::SHOTGUN) return;
	if (ptr_local_entity->ammunition == 0) return;
	if (ptr_local_entity->shots_fired == 0) return;
	if (Utility::ClockDiff(std::clock(), t_last_compensation) < ptr_active_weapon->GetActiveConfig()->rcs_sync_pause) return;
	t_last_compensation = std::clock();

	Axis diff = ptr_local_entity->local_punch.SubtractNorm(punch_backup);
	punch_backup = ptr_local_entity->local_punch;
	diff.MultiplyDirect(2.f * ptr_active_weapon->GetActiveConfig()->rcs_reduction_factor);

	if (ptr_local_entity->shots_fired == 0) return;

	if (Utility::ClockDiff(std::clock(), t_random_yaw) > 315)
	{
		tracking_value_yaw = 0.7f + 0.6f * static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		t_random_yaw = std::clock();
	}
	if (Utility::ClockDiff(std::clock(), t_random_pitch) > 415)
	{
		tracking_value_pitch = 0.8f + 0.4f * (float)std::rand() / (float)RAND_MAX;
		t_random_pitch = std::clock();
	}
	diff.yaw *= tracking_value_yaw;
	diff.pitch *= tracking_value_pitch;

	Memory::SubtractFromViewAngle(diff);
}

void RecoilControl::Reset()
{
	punch_backup = ptr_local_entity->local_punch;
}