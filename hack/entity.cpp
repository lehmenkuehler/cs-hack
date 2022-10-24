#include <bitset>

#include "entity.h"
#include "vector.h"
#include "axis.h"
#include "utility.h"

Entity::Entity()
{
	entity_base = 0;
	active_weapon_entity = 0;
	t_spot_local_entity = std::clock();
	t_spot_team = std::clock();
	init = false;
	player = false;
	active_weapon_item_index = 0;
	armor = 0;
	armored = 0;
	ammunition = 0;
	class_id = 0;
	color_id = 0;
	crosshair_id = 0;
	crouching = 0;
	dormant = 1;
	health = 100;
	id = 0;
	immune = 0;
	in_scope = 0;
	last_weapon_item_index = 0;
	life_state = 1;
	on_ground = 1;
	reloading = 0;
	shots_fired = 0;
	shots_fired_last_tick = 0;
	spotted_by = 0;
	spotted_by_local_entity = 0;
	t_acceleration_initiated = std::clock();
	t_crouching = std::clock();
	t_deceleration_initiated = std::clock();
	t_move_registered = std::clock();
	t_last_weapon_change = std::clock();
	t_not_in_scope = std::clock();
	t_on_ground = std::clock();
	t_shot_fired = std::clock();
	team = 1;
	velocity = 0;
}

void Entity::Update(int index, int* hostile_id)
{
	id = index;
	if (!Memory::GetValidEntity(index, &entity_base))
	{
		init = false;
		return;
	}
	init = true;

	Memory::GetEntityClassID(entity_base, &class_id);
	if (class_id != 40)
	{
		player = false;
		return;
	}

	player = true;

	Memory::Get(entity_base, ptr_offset->dormant, &dormant);
	Memory::Get(entity_base, ptr_offset->health, &health);
	Memory::Get(entity_base, ptr_offset->life_state, &life_state);
	Memory::Get(entity_base, ptr_offset->team_num, &team);
	Memory::Get(entity_base, ptr_offset->crosshair_id, &crosshair_id);

	if (ptr_local_entity != nullptr && team != ptr_local_entity->team)
	{
		*hostile_id++;
		color_id = *hostile_id % 5;
	}
	else color_id = -1;

	bool last_tick_sbt = spotted_by != 0;
	bool last_tick_sble = spotted_by_local_entity;

	Memory::Get(entity_base, ptr_offset->spotted_by_mask, &spotted_by);
	std::bitset<32> bits(spotted_by);
	if (ptr_local_entity != nullptr) spotted_by_local_entity = bits[ptr_local_entity->id - 1] != 0;

	if (!last_tick_sble && spotted_by_local_entity) t_spot_local_entity = std::clock();
	if (!last_tick_sbt && spotted_by != 0) t_spot_team = std::clock();

	shots_fired_last_tick = shots_fired;
	Memory::Get(entity_base, ptr_offset->shots_fired, &shots_fired);
	if (shots_fired > 0) t_shot_fired = std::clock();
	Memory::Get(entity_base, ptr_offset->armor_value, &armor);
	armored = armor > 0;
	Memory::Get(entity_base, ptr_offset->gun_game_immunity, &immune);
	Memory::Get(entity_base, ptr_offset->is_scoped, &in_scope);
	if (!in_scope) t_not_in_scope = std::clock();

	int flags;
	Memory::Get(entity_base, ptr_offset->flags, &flags);

	bool on_ground_last_tick = on_ground;
	bool crouching_last_tick = crouching;

	on_ground = flags & 1 << 0;
	crouching = flags & 1 << 1;

	if (crouching_last_tick != crouching) t_crouching = std::clock();
	if (on_ground_last_tick != on_ground) t_on_ground = std::clock();

	Memory::Get(entity_base, ptr_offset->vec_origin, &origin);
	Memory::Get(entity_base, ptr_offset->vec_velocity, &velocity_vector);

	float velocity_last_tick = velocity;
	velocity = velocity_vector.Abs();

	if (velocity_last_tick < 5.f && velocity >= 5.f) t_acceleration_initiated = std::clock();
	if (velocity_last_tick >= 180.f && velocity < 180.f) t_deceleration_initiated = std::clock();

	Memory::GetBoneMatrixVector(entity_base, HEAD, &origin_head);
	Memory::GetBoneMatrixVector(entity_base, CHEST, &origin_chest);
	Memory::GetBoneMatrixVector(entity_base, STOMACH, &origin_stomach);

	DWORD active_weapon_handle;
	Memory::Get(entity_base, ptr_offset->active_weapon, &active_weapon_handle);
	Memory::GetEntityFromHandle(active_weapon_handle, &active_weapon_entity);
	Memory::Get(active_weapon_entity, ptr_offset->item_definition_index, &active_weapon_item_index);

	if (active_weapon_item_index != last_weapon_item_index) t_last_weapon_change = std::clock();
	last_weapon_item_index = active_weapon_item_index;

	if (Memory::GetLocalEntity(entity_base))
	{
		ptr_local_entity = this;
		Memory::Get(active_weapon_entity, ptr_offset->in_reload, &reloading);
		Memory::Get(active_weapon_entity, ptr_offset->clip, &ammunition);
		Memory::GetViewAngle(&local_view);
		Memory::Get(entity_base, ptr_offset->aim_punch_angle, &local_punch);
		Memory::Get(entity_base, ptr_offset->aim_punch_angle_vel, &local_punch_velocity);

		Vector view_offset;
		Memory::Get(entity_base, ptr_offset->vec_view_offset, &view_offset);
		origin_view = origin + view_offset;

		if (local_view_last_tick.pitch != local_view.pitch || local_view_last_tick.yaw != local_view.yaw ||
			origin_last_tick.x != origin.x || origin_last_tick.y != origin.y || origin_last_tick.z != origin.z) t_move_registered = std::clock();

		local_view_last_tick = local_view;
		origin_last_tick = origin;
	}
}

DWORD Entity::GetEntityBase()
{
	return entity_base;
}

DWORD Entity::GetWeaponEntity()
{
	return active_weapon_entity;
}