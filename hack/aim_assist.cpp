#include <cmath>
#include <bitset>

#include "aim_assist.h"
#include "utility.h"
#include "entity.h"
#include "weapon.h"

AimAssist::AimAssist()
{
	target_id = -1;
	target_bone = HEAD;
	t_target_killed = 0;
	t_last_tick = std::clock();
	distance = 2000.f;
	active_last_tick = false;
	t_activation = std::clock();
	t_last_velocity_update = std::clock();
	t_start = std::clock();
}

void AimAssist::Activate()
{
	if (!active_last_tick) t_activation = std::clock();
	active_last_tick = true;

	if (ptr_active_weapon->weapon_type == WeaponType::KNIFE || ptr_active_weapon->weapon_type == WeaponType::GRENADE || ptr_active_weapon->weapon_type == WeaponType::BOMB ||
		(ptr_active_weapon->weapon_type == WeaponType::SNIPER_RIFLE && !ptr_local_entity->in_scope))
	{
		t_last_tick = std::clock();
		return;
	}

	if (target_id != -1 && (ptr_entity_list + target_id)->life_state != LIFE_STATE_ALIVE)
	{
		t_target_killed = std::clock();
		target_id = -1;
	}

	if (Search(&target_id)) ConvergeViewAngle();
	t_last_tick = std::clock();
}

void AimAssist::Reset()
{
	active_last_tick = false;
}

void AimAssist::ConvergeViewAngle()
{
	Axis d_omega = Axis(150.f, 150.f);
	if (ptr_local_entity->shots_fired == 0) d_omega.MultiplyDirect(ptr_active_weapon->GetActiveConfig()->aim_assist_tracking_speed);
	else d_omega.MultiplyDirect(ptr_active_weapon->GetActiveConfig()->aim_assist_tracking_speed_fire);

	Axis local_view = ptr_local_entity->local_view;
	Axis target_view;

	float target_velocity = (ptr_entity_list + target_id)->velocity;
	if (target_velocity > 250.f) target_velocity = 250.f;

	distance = (ptr_local_entity->origin - (ptr_entity_list + target_id)->origin).Abs();

	float pred = static_cast<float>(ptr_active_weapon->GetActiveConfig()->aim_assist_movement_prediction) * target_velocity / 1000.f;
	Vector movement_prediction = (ptr_entity_list + target_id)->velocity_vector.UnitVector() * pred;

	if (ptr_active_weapon->GetActiveConfig()->aim_assist_force_headshot && ptr_local_entity->shots_fired <= ptr_active_weapon->GetActiveConfig()->aim_assist_force_body_after_shots)
	{
		target_view = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_head - movement_prediction);
	}
	else if (ptr_local_entity->shots_fired > ptr_active_weapon->GetActiveConfig()->aim_assist_force_body_after_shots)
	{
		target_view = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_chest - movement_prediction);
	}
	else
	{
		Axis target_view_head = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_head - movement_prediction);
		Axis target_view_chest = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_chest - movement_prediction);
		Axis target_view_stomach = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + target_id)->origin_stomach - movement_prediction);

		if (local_view.DistanceAbs(target_view_head).Abs() < local_view.DistanceAbs(target_view_chest).Abs())
		{
			target_bone = HEAD;
			target_view = target_view_head;
		}
		else
		{
			target_bone = CHEST;
			target_view = target_view_chest;
		}
	}

	target_view.SubtractDirect(ptr_local_entity->local_punch.Multiply(2.f * ptr_active_weapon->GetActiveConfig()->aim_assist_punch));
	Axis diff = target_view.DistanceAbs(local_view);
	float diff_angle = diff.Abs();

	float hitbox = HitboxRadius();

	if (distance < 20.f) return;
	if (ptr_local_entity->ammunition == 0) return;
	if (ptr_local_entity->reloading) return;

	if (Utility::ClockDiff(std::clock(), t_activation) < ptr_active_weapon->GetActiveConfig()->aim_assist_activation_softening)
	{
		double progress = static_cast<double>(Utility::ClockDiff(std::clock(), t_activation)) / static_cast<double>(ptr_active_weapon->GetActiveConfig()->aim_assist_activation_softening);
		d_omega.MultiplyDirect(static_cast<float>(progress));
	}

	if (Utility::ClockDiff(std::clock(), (ptr_entity_list + target_id)->t_crouching) < ptr_active_weapon->GetActiveConfig()->aim_assist_crouch_delay)
	{
		double progress = static_cast<double>(Utility::ClockDiff(std::clock(), (ptr_entity_list + target_id)->t_crouching)) / static_cast<double>(ptr_active_weapon->GetActiveConfig()->aim_assist_crouch_delay);
		d_omega.MultiplyDirect(static_cast<float>(progress));
	}

	if (Utility::ClockDiff(std::clock(), (ptr_entity_list + target_id)->t_on_ground) < ptr_active_weapon->GetActiveConfig()->aim_assist_jump_delay)
	{
		double progress = static_cast<double>(Utility::ClockDiff(std::clock(), (ptr_entity_list + target_id)->t_on_ground)) / static_cast<double>(ptr_active_weapon->GetActiveConfig()->aim_assist_jump_delay);
		d_omega.MultiplyDirect(static_cast<float>(progress));
	}

	if (ptr_active_weapon->weapon_type == WeaponType::SNIPER_RIFLE && Utility::ClockDiff(std::clock(), ptr_local_entity->t_not_in_scope) < 200)
	{
		double progress = static_cast<double>(Utility::ClockDiff(std::clock(), ptr_local_entity->t_not_in_scope)) / 200.0;
		d_omega.MultiplyDirect(static_cast<float>(progress));
	}

	if (ptr_local_entity->shots_fired == 0 && Utility::ClockDiff(std::clock(), (ptr_entity_list + target_id)->t_spot_local_entity) < ptr_active_weapon->GetActiveConfig()->aim_assist_spot_delay)
	{
		double progress = static_cast<double>(
			Utility::ClockDiff(std::clock(), (ptr_entity_list + target_id)->t_spot_local_entity)) /
			static_cast<double>(ptr_active_weapon->GetActiveConfig()->aim_assist_spot_delay);
		d_omega.MultiplyDirect(static_cast<float>(std::pow(progress, 2)));
	}

	if (Utility::ClockDiff(std::clock(), t_target_killed) < ptr_active_weapon->GetActiveConfig()->aim_assist_transfer_pause)
	{
		double progress = static_cast<double>(
			Utility::ClockDiff(std::clock(), t_target_killed)) / static_cast<double>(ptr_active_weapon->GetActiveConfig()->aim_assist_transfer_pause);
		d_omega.MultiplyDirect(static_cast<float>(std::pow(progress, 2)));
	}

	if (Utility::ClockDiff(std::clock(), ptr_local_entity->t_last_weapon_change) < ptr_active_weapon->time) return;

	if (ptr_local_entity->velocity > ptr_active_weapon->GetActiveConfig()->aim_assist_maximum_velocity) return;
	else if (ptr_active_weapon->GetActiveConfig()->aim_assist_linear_velocity_mapping) d_omega.MultiplyDirect(1.f - ptr_local_entity->velocity / ptr_active_weapon->GetActiveConfig()->aim_assist_maximum_velocity);

	if (!ptr_local_entity->on_ground) d_omega.MultiplyDirect(0.3f);
	if (!(ptr_entity_list + target_id)->on_ground) d_omega.MultiplyDirect(0.4f);

	int pause = ptr_active_weapon->GetActiveConfig()->aim_assist_fire_pause;
	pause += static_cast<int>(ptr_active_weapon->GetActiveConfig()->aim_assist_pause_range_extension * distance);
	int time_since_shot = Utility::ClockDiff(std::clock(), ptr_local_entity->t_shot_fired);

	if (ptr_local_entity->shots_fired == 0 && pause != 0 && time_since_shot < pause)
	{
		double progress = static_cast<double>(time_since_shot) / static_cast<double>(pause);
		d_omega.MultiplyDirect(static_cast<float>(std::pow(progress, 2)));
	}

	float ratio = std::abs((local_view.pitch - target_view.pitch) / (local_view.yaw - target_view.yaw));
	d_omega.yaw = d_omega.Abs() * std::sqrt(1 / (1 + ratio * ratio));
	d_omega.pitch = d_omega.yaw * ratio;

	if (diff_angle < hitbox * ptr_active_weapon->GetActiveConfig()->aim_assist_curb_zone) d_omega.MultiplyDirect(diff_angle / (hitbox * ptr_active_weapon->GetActiveConfig()->aim_assist_curb_zone));
	if (diff_angle < hitbox) d_omega.MultiplyDirect(static_cast<float>(diff_angle / hitbox));

	d_omega.MultiplyDirect(std::atan(300.f / distance));

	float distance_factor = 1.f - (distance / 4000.f) * ptr_active_weapon->GetActiveConfig()->aim_assist_distance_reduction;
	if (distance_factor < 0.1f) distance_factor = 0.1f;
	d_omega.MultiplyDirect(distance_factor);

	d_omega.MultiplyDirect(static_cast<float>(Utility::ClockDiff(std::clock(), t_last_tick)) / 1000.f);

	if (d_omega.pitch > 0.9f) d_omega.pitch = 0.9f;
	if (d_omega.yaw > 0.9f) d_omega.yaw = 0.9f;

	if (local_view.ReferenceEntityRightsided(target_view)) d_omega.yaw *= -1.f;
	if (local_view.yaw < -90.f && target_view.yaw > 90.f) d_omega.yaw *= -1.f;
	if (local_view.ReferenceEntityHigher(target_view)) d_omega.pitch *= -1.f;

	Memory::SubtractFromViewAngle(d_omega);
}

float AimAssist::HitboxRadius()
{
	float width;
	switch (target_bone)
	{
	case HEAD:
		width = 3.f;
		break;
	case CHEST:
		width = 8.f;
		break;
	case STOMACH:
		width = 6.f;
		break;
	default:
		width = 8.f;
		break;
	}
	return std::atan(width / distance) * 180.f / PI;
}

bool AimAssist::Search(int* id)
{
	float diff = 99999.f;
	int tmp_target_id = -1;

	if (ptr_active_weapon->GetActiveConfig()->aim_assist_stick_to_target &&
		ptr_local_entity->shots_fired != 0 &&
		(ptr_entity_list + *id) != 0 &&
		(ptr_entity_list + *id)->init &&
		(ptr_entity_list + *id)->spotted_by_local_entity) return true;

	for (int i = 0; i <= 256; i++)
	{
		if ((ptr_entity_list + i) == 0) continue;
		if (!(ptr_entity_list + i)->init) continue;

		if ((ptr_active_weapon->GetActiveConfig()->aim_assist_friendly_fire || (ptr_entity_list + i)->team != ptr_local_entity->team) && (ptr_entity_list + i)->dormant == 0 && (ptr_entity_list + i)->life_state == LIFE_STATE_ALIVE && !(ptr_entity_list + i)->immune)
		{
			if (ptr_active_weapon->GetActiveConfig()->aim_assist_visibility_check && !(ptr_entity_list + i)->spotted_by_local_entity) continue;

			float comp_val;
			comp_val = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + i)->origin).DistanceAbs(ptr_local_entity->local_view).yaw;
			if (comp_val < diff)
			{
				tmp_target_id = i;
				diff = comp_val;
			}
		}
	}

	if (tmp_target_id == -1)
	{
		*id = -1;
		return false;
	}

	Axis target_view = Vector::AxisFromVector(ptr_local_entity->origin_view - (ptr_entity_list + tmp_target_id)->origin_head);

	float fov = 0.f;
	if (ptr_active_weapon->GetActiveConfig()->aim_assist_field_of_view_type == FOV_TYPE_DEGREE) fov = ptr_active_weapon->GetActiveConfig()->aim_assist_field_of_view;
	else if (ptr_active_weapon->GetActiveConfig()->aim_assist_field_of_view_type == FOV_TYPE_SOURCE_ENGINE_UNITS)
		fov = std::atan(ptr_active_weapon->GetActiveConfig()->aim_assist_field_of_view / (ptr_local_entity->origin - (ptr_entity_list + tmp_target_id)->origin).Abs()) * 180.f / PI;

	if (target_view.DistanceAbs(ptr_local_entity->local_view).Abs() < fov / 2.f)
	{
		*id = tmp_target_id;
		return true;
	}
	else
	{
		*id = -1;
		return false;
	}
}