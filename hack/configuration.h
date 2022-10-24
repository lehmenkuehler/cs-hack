#pragma once
#include <vector>

#include "glow_data.h"
#include "button.h"

constexpr short FOV_TYPE_DEGREE = 0;
constexpr short FOV_TYPE_SOURCE_ENGINE_UNITS = 1;

// KEY CODES https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx

struct Configuration
{
	bool variant_hold_key = true;
	bool variant_play_sound = false;
	short variant_key = -1;
	Button button_variant = Button(variant_key, variant_hold_key, 0);

	int load_config_key = 0x2D;
	int panic_key = 0x23;
	Button button_load_config = Button(load_config_key, true, false);
	Button button_panic = Button(panic_key, true, false);
	bool activate_console = true;
	bool hide_console_after_startup = false;
	int sync_break = 10;

	bool aim_assist_on = false;
	int aim_assist_key = VK_F5;
	bool aim_assist_hold_key = false;
	bool aim_assist_play_sound = false;
	int aim_assist_field_of_view_type = FOV_TYPE_DEGREE;
	float aim_assist_field_of_view = 10.f;
	float aim_assist_tracking_speed = 0.f;
	float aim_assist_tracking_speed_fire = 0.20f;
	float aim_assist_curb_zone = 2.f;
	float aim_assist_punch = 1.f;
	float aim_assist_distance_reduction = 1.f;
	float aim_assist_maximum_velocity = 300.f;
	bool aim_assist_linear_velocity_mapping = true;
	bool aim_assist_stick_to_target = false;
	bool aim_assist_visibility_check = true;
	bool aim_assist_friendly_fire = false;
	bool aim_assist_force_headshot = false;
	int aim_assist_force_body_after_shots = 150;
	int aim_assist_jump_delay = 0;
	int aim_assist_crouch_delay = 0;
	int aim_assist_movement_prediction = 0;
	int aim_assist_transfer_pause = 0;
	int aim_assist_fire_pause = 0;
	float aim_assist_pause_range_extension = 0.f;
	int aim_assist_spot_delay = 0;
	int aim_assist_activation_softening = 0;
	Button button_aim_assist = Button(aim_assist_key, aim_assist_hold_key, aim_assist_on);

	bool triggerbot_on = false;
	int triggerbot_key = VK_F6;
	bool triggerbot_hold_key = true;
	bool triggerbot_play_sound = false;
	int triggerbot_key_spraydown = 0x06;
	int triggerbot_mode = 0;
	int triggerbot_pause_after_shot = 0;
	float triggerbot_pause_range_extension_factor = 0.f;
	int triggerbot_fake_shot_duration = 200;
	int triggerbot_fire_extension = 200;
	int triggerbot_delay = 0;
	float triggerbot_maximum_distance = 7000.f;
	float triggerbot_maximum_velocity = 270.f;
	bool triggerbot_disable_while_entering_scope = false;
	bool triggerbot_enable_in_scope_only = false;
	bool triggerbot_head_only = false;
	bool triggerbot_friendly_fire = false;
	bool triggerbot_visibility_check = true;
	Button button_triggerbot = Button(triggerbot_key, triggerbot_hold_key, triggerbot_on);
	Button button_triggerbot_spraydown = Button(triggerbot_key_spraydown, triggerbot_hold_key, triggerbot_on);

	bool glow_esp_on = false;
	int glow_esp_key = VK_F7;
	bool glow_esp_hold_key = false;
	bool glow_esp_play_sound = true;
	bool glow_esp_toggle_for_all_weapons = true;
	bool glow_esp_spotted_only = false;
	bool glow_esp_distinct_colors = false;
	float glow_esp_distinct_mix_factor = 1.f;
	long long glow_esp_rgba_friendly = 0x0078FFC8;
	long long glow_esp_rgba_friendly_1hp = 0xFF000008C;
	long long glow_esp_rgba_hostile = 0xFFFFFF8C;
	long long glow_esp_rgba_hostile_1hp = 0xFF000008C;
	long long glow_esp_rgba_weapon = 0x7045E6BE;
	long long glow_esp_rgba_grenade = 0xC245E6A0;
	long long glow_esp_rgba_bomb = 0xF48071FF;
	long long glow_esp_rgba_money = 0x44AA00A0;
	Button button_glow_esp = Button(glow_esp_key, glow_esp_hold_key, glow_esp_on);

	bool rcs_on = false;
	int rcs_key = VK_F8;
	bool rcs_hold_key = false;
	bool rcs_play_sound = true;
	float rcs_reduction_factor = 0.70f;
	int rcs_sync_pause = 20;
	Button button_rcs = Button(rcs_key, rcs_hold_key, rcs_on);

	bool aimbot_on = false;
	int aimbot_key = 0x64;
	bool aimbot_hold_key = true;
	bool aimbot_play_sound = false;
	float aimbot_field_of_view = 10.f;
	bool aimbot_single_shot = false;
	bool aimbot_fire_if_no_target = false;
	bool aimbot_visibility_check = true;
	bool aimbot_friendly_fire = false;
	Button button_aimbot = Button(aimbot_key, aimbot_hold_key, aimbot_on);

	GlowData glow_team = GlowData(glow_esp_rgba_friendly);
	GlowData glow_team_1hp = GlowData(glow_esp_rgba_friendly_1hp);
	GlowData glow_hostile = GlowData(glow_esp_rgba_hostile);
	GlowData glow_hostile_1hp = GlowData(glow_esp_rgba_hostile_1hp);
	GlowData glow_weapon = GlowData(glow_esp_rgba_weapon);
	GlowData glow_grenade = GlowData(glow_esp_rgba_grenade);
	GlowData glow_bomb = GlowData(glow_esp_rgba_bomb);
	GlowData glow_money = GlowData(glow_esp_rgba_money);
	std::vector<GlowData> distinct_colors{ GlowData(0xFFF700AA), GlowData(0x7B00FFAA), GlowData(0x00FF09AA), GlowData(0x00D6FFAA), GlowData(0xFFFFFFAA) };

	void updateConfigurationObjects()
	{
		glow_team = GlowData(glow_esp_rgba_friendly);
		glow_team_1hp = GlowData(glow_esp_rgba_friendly_1hp);
		glow_hostile = GlowData(glow_esp_rgba_hostile);
		glow_hostile_1hp = GlowData(glow_esp_rgba_hostile_1hp);
		glow_weapon = GlowData(glow_esp_rgba_weapon);
		glow_grenade = GlowData(glow_esp_rgba_grenade);
		glow_bomb = GlowData(glow_esp_rgba_bomb);
		glow_money = GlowData(glow_esp_rgba_money);

		button_variant = Button(variant_key, variant_hold_key, 0);
		button_load_config = Button(load_config_key, true, false);
		button_aim_assist = Button(aim_assist_key, aim_assist_hold_key, aim_assist_on);
		button_glow_esp = Button(glow_esp_key, glow_esp_hold_key, glow_esp_on);
		button_triggerbot = Button(triggerbot_key, triggerbot_hold_key, triggerbot_on);
		button_triggerbot_spraydown = Button(triggerbot_key_spraydown, triggerbot_hold_key, triggerbot_on);
		button_aimbot = Button(aimbot_key, aimbot_hold_key, aimbot_on);
		button_rcs = Button(rcs_key, rcs_hold_key, rcs_on);
	}
};