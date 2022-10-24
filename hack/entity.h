#pragma once
#include <ctime>

#include "memory.h"
#include "offsets.h"
#include "vector.h"
#include "axis.h"

constexpr short LIFE_STATE_ALIVE = 0;
constexpr short LIFE_STATE_DYING = 1;
constexpr short LIFE_STATE_DEAD = 2;

constexpr short HEAD = 8;
constexpr short CHEST = 6;
constexpr short STOMACH = 3;

class Entity
{
public:
	Entity();
	bool init;
	bool player;

	void Update(int index, int* hostile_id);

	int id;
	int color_id;
	int class_id;

	int health;
	int life_state;
	int team;
	int crosshair_id;
	int dormant;
	int spotted_by;
	bool spotted_by_local_entity;
	short active_weapon_item_index;

	int shots_fired;
	int shots_fired_last_tick;

	int armor;
	bool armored;
	bool immune;
	bool in_scope;

	bool on_ground;
	bool crouching;
	std::clock_t t_on_ground;
	std::clock_t t_crouching;

	std::clock_t t_acceleration_initiated;
	std::clock_t t_deceleration_initiated;

	std::clock_t t_spot_local_entity;
	std::clock_t t_spot_team;
	std::clock_t t_not_in_scope;
	std::clock_t t_shot_fired;

	std::clock_t t_last_weapon_change;
	short last_weapon_item_index;

	Vector origin;
	Vector origin_head;
	Vector origin_chest;
	Vector origin_stomach;

	Vector origin_key;
	Vector origin_head_key;
	Vector origin_chest_key;
	Vector origin_stomach_key;

	Vector velocity_vector;
	float velocity;

	Axis local_view;
	Axis local_punch;
	Axis local_punch_velocity;
	Vector origin_view;

	Axis local_view_last_tick;
	Vector origin_last_tick;
	bool reloading;
	int ammunition;

	std::clock_t t_move_registered;

	DWORD GetEntityBase();
	DWORD GetWeaponEntity();

private:
	DWORD entity_base;
	DWORD active_weapon_entity;
};

extern Entity* ptr_local_entity;
extern Entity* ptr_entity_list;