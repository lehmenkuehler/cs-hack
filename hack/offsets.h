#pragma once
#include <Windows.h>

// https://github.com/frk1/hazedumper

struct Offset
{
	ptrdiff_t entity_span = 0x10;
	ptrdiff_t dormant = 0xED;
	ptrdiff_t armor_value = 0;
	ptrdiff_t aim_punch_angle = 0;
	ptrdiff_t aim_punch_angle_vel = 0;
	ptrdiff_t gun_game_immunity = 0;
	ptrdiff_t in_reload = 0;
	ptrdiff_t is_scoped = 0;
	ptrdiff_t spotted_by_mask = 0;
	ptrdiff_t dw_bone_matrix = 0;
	ptrdiff_t flags = 0;
	ptrdiff_t active_weapon = 0;
	ptrdiff_t clip = 0;
	ptrdiff_t crosshair_id = 0;
	ptrdiff_t health = 0;
	ptrdiff_t item_definition_index = 0;
	ptrdiff_t shots_fired = 0;
	ptrdiff_t team_num = 0;
	ptrdiff_t life_state = 0;
	ptrdiff_t vec_origin = 0;
	ptrdiff_t vec_velocity = 0;
	ptrdiff_t vec_view_offset = 0;
	ptrdiff_t dw_client_state = 0;
	ptrdiff_t dw_client_state_is_hltv = 0;
	ptrdiff_t dw_client_state_state = 0;
	ptrdiff_t dw_client_state_view_angles = 0;
	ptrdiff_t dw_entity_list = 0;
	ptrdiff_t dw_force_attack = 0;
	ptrdiff_t dw_force_backward = 0;
	ptrdiff_t dw_force_forward = 0;
	ptrdiff_t dw_force_jump = 0;
	ptrdiff_t dw_force_left = 0;
	ptrdiff_t dw_force_right = 0;
	ptrdiff_t dw_glow_object_manager = 0;
	ptrdiff_t dw_local_player = 0;
};

extern Offset* ptr_offset;