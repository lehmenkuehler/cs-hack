#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <shlobj.h>
#include <fstream>
#include <sys/stat.h>
#include <unordered_map>
#include <filesystem>
#include <thread>
#include <chrono>

#include "parser.h"
#include "equipment.h"
#include "offsets.h"
#include "output.h"

#include "./includes/json.hpp"

#pragma warning(push, 0)        
#pragma warning(pop)

namespace fs = std::filesystem;
using json = nlohmann::json;

bool Parser::GetConfigPath(string* path, string file_name)
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	if (result != S_OK)	return false;

	string path_documents(my_documents);
	path_documents.append("\\Anda");
	if (!fs::exists(path_documents))
	{
		fs::create_directory(path_documents);
		Output::PrintTime();
		Output::PrintToConsole("Configuration directory created ", WHITE, BLACK);
		Output::PrintToConsole("(" + path_documents + ")", 1, GRAY, BLACK);
	}
	*path = path_documents + "\\" + file_name;
	return true;
}

short Parser::ParseKey(string key)
{
	if (key == "LMB")  return 0x01;
	else if (key == "RMB")  return 0x02;
	else if (key == "CANCEL")  return 0x03;
	else if (key == "MMB")  return 0x04;
	else if (key == "MB1") return 0x05;
	else if (key == "MB2") return 0x06;
	else if (key == "BACKSPACE") return 0x08;
	else if (key == "TAB") return 0x09;
	else if (key == "CLEAR") return 0x0C;
	else if (key == "ENTER") return 0x0D;
	else if (key == "SHIFT") return 0x10;
	else if (key == "CTRL") return 0x11;
	else if (key == "ALT") return 0x12;
	else if (key == "PAUSE") return 0x13;
	else if (key == "CAPS_LOCK") return 0x14;
	else if (key == "ESC") return 0x1B;
	else if (key == "SPACE") return 0x20;
	else if (key == "PAGE_UP") return 0x21;
	else if (key == "PAGE_DOWN") return 0x22;
	else if (key == "END") return 0x23;
	else if (key == "HOME") return 0x24;
	else if (key == "LEFT_ARROW") return 0x25;
	else if (key == "UP_ARROW") return 0x26;
	else if (key == "RIGHT_ARROW") return 0x27;
	else if (key == "DOWN_ARROW") return 0x28;
	else if (key == "PRINT") return 0x2A;
	else if (key == "INSERT") return 0x2D;
	else if (key == "DELETE") return 0x2E;
	else if (key == "HELP") return 0x2F;
	else if (key == "0") return 0x30;
	else if (key == "1") return 0x31;
	else if (key == "2") return 0x32;
	else if (key == "3") return 0x33;
	else if (key == "4") return 0x34;
	else if (key == "5") return 0x35;
	else if (key == "6") return 0x36;
	else if (key == "7") return 0x37;
	else if (key == "8") return 0x38;
	else if (key == "9") return 0x39;
	else if (key == "A") return 0x41;
	else if (key == "B") return 0x42;
	else if (key == "C") return 0x43;
	else if (key == "D") return 0x44;
	else if (key == "E") return 0x45;
	else if (key == "F") return 0x46;
	else if (key == "G") return 0x47;
	else if (key == "H") return 0x48;
	else if (key == "I") return 0x49;
	else if (key == "J") return 0x4A;
	else if (key == "K") return 0x4B;
	else if (key == "L") return 0x4C;
	else if (key == "M") return 0x4D;
	else if (key == "N") return 0x4E;
	else if (key == "O") return 0x4F;
	else if (key == "P") return 0x50;
	else if (key == "Q") return 0x51;
	else if (key == "R") return 0x52;
	else if (key == "S") return 0x53;
	else if (key == "T") return 0x54;
	else if (key == "U") return 0x55;
	else if (key == "V") return 0x56;
	else if (key == "W") return 0x57;
	else if (key == "X") return 0x58;
	else if (key == "Y") return 0x59;
	else if (key == "Z") return 0x5A;
	else if (key == "F1") return VK_F1;
	else if (key == "F2") return VK_F2;
	else if (key == "F3") return VK_F3;
	else if (key == "F4") return VK_F4;
	else if (key == "F5") return VK_F5;
	else if (key == "F6") return VK_F6;
	else if (key == "F7") return VK_F7;
	else if (key == "F8") return VK_F8;
	else if (key == "F9") return VK_F9;
	else if (key == "F10") return VK_F10;
	else if (key == "F11") return VK_F11;
	else if (key == "F12") return VK_F12;
	else if (key == "SCROLL_LOCK") return VK_SCROLL;
	else if (key == "MENU") return VK_LMENU;
	else if (key == ";") return VK_OEM_1;
	else if (key == "=") return VK_OEM_PLUS;
	else if (key == ",") return VK_OEM_COMMA;
	else if (key == "-") return VK_OEM_MINUS;
	else if (key == ".") return VK_OEM_PERIOD;
	else if (key == "/") return VK_OEM_2;
	else if (key == "`") return VK_OEM_3;
	else if (key == "[") return VK_OEM_4;
	else if (key == "\"") return VK_OEM_5;
	else if (key == "]") return VK_OEM_6;
	else if (key == "'") return VK_OEM_7;
	else if (key == "NUMPAD_1") return 0x61;
	else if (key == "NUMPAD_2") return 0x62;
	else if (key == "NUMPAD_3") return 0x63;
	else if (key == "NUMPAD_4") return 0x64;
	else if (key == "NUMPAD_5") return 0x65;
	else if (key == "NUMPAD_6") return 0x66;
	else if (key == "NUMPAD_7") return 0x67;
	else if (key == "NUMPAD_8") return 0x68;
	else if (key == "NUMPAD_9") return 0x69;
	else if (key == "MULTIPLY") return 0x6A;
	else if (key == "ADD") return 0x6B;
	else if (key == "SUBTRACT") return 0x6D;
	else if (key == "DECIMAL") return 0x6E;
	else if (key == "DIVIDE") return 0X6F;
	else if (key == "LEFT_SHIFT") return 0XA0;
	else if (key == "RIGHT_SHIFT") return 0xA1;
	else if (key == "LEFT_CTRL") return 0xA2;
	else if (key == "RIGHT_CTRL") return 0xA3;
	else if (key == "LEFT_WINDOWS") return 0xA4;
	else if (key == "RIGHT_WINDOWS") return 0xA5;
	else if (key == "CLEAR") return 0xFE;
	else  return (short)std::stoul(key, nullptr, 16);
}

void Parser::SetValue(Configuration* config, string name, string value)
{
	if (name == "LoadConfigKey") config->load_config_key = ParseKey(value);
	else if (name == "PanicKey") config->panic_key = ParseKey(value);
	else if (name == "SyncBreak") config->sync_break = stoi(value);
	else if (name == "ActivateConsole") config->activate_console = String2Bool(value);
	else if (name == "HideConsoleAfterStartup") config->hide_console_after_startup = String2Bool(value);
	else if (name == "Variant::HoldKey") config->variant_hold_key = String2Bool(value);
	else if (name == "Variant::PlaySound") config->variant_play_sound = String2Bool(value);	

	else if (name == "AimAssist::On") config->aim_assist_on = String2Bool(value);
	else if (name == "AimAssist::Key") config->aim_assist_key = ParseKey(value);
	else if (name == "AimAssist::HoldKey") config->aim_assist_hold_key = String2Bool(value);
	else if (name == "AimAssist::PlaySound") config->aim_assist_play_sound = String2Bool(value);
	else if (name == "AimAssist::FieldOfViewType") config->aim_assist_field_of_view_type = stoi(value);
	else if (name == "AimAssist::FieldOfView") config->aim_assist_field_of_view = stof(value);
	else if (name == "AimAssist::TrackingSpeed") config->aim_assist_tracking_speed = stof(value);
	else if (name == "AimAssist::TrackingSpeedFire") config->aim_assist_tracking_speed_fire = stof(value);
	else if (name == "AimAssist::CurbZone") config->aim_assist_curb_zone = stof(value);
	else if (name == "AimAssist::Punch") config->aim_assist_punch = stof(value);
	else if (name == "AimAssist::DistanceReduction") config->aim_assist_distance_reduction = stof(value);
	else if (name == "AimAssist::MaximumVelocity") config->aim_assist_maximum_velocity = stof(value);
	else if (name == "AimAssist::LinearVelocityMapping") config->aim_assist_linear_velocity_mapping = String2Bool(value);
	else if (name == "AimAssist::StickToTarget") config->aim_assist_stick_to_target = String2Bool(value);
	else if (name == "AimAssist::VisibilityCheck") config->aim_assist_visibility_check = String2Bool(value);
	else if (name == "AimAssist::FriendlyFire") config->aim_assist_friendly_fire = String2Bool(value);
	else if (name == "AimAssist::ForceHeadshot") config->aim_assist_force_headshot = String2Bool(value);
	else if (name == "AimAssist::ForceBodyAfterShots") config->aim_assist_force_body_after_shots = stoi(value);
	else if (name == "AimAssist::JumpDelay") config->aim_assist_jump_delay = stoi(value);
	else if (name == "AimAssist::CrouchDelay") config->aim_assist_crouch_delay = stoi(value);
	else if (name == "AimAssist::MovementPrediction") config->aim_assist_movement_prediction = stoi(value);
	else if (name == "AimAssist::TransferPause") config->aim_assist_transfer_pause = stoi(value);
	else if (name == "AimAssist::FirePause") config->aim_assist_fire_pause = stoi(value);
	else if (name == "AimAssist::PauseRangeExtensionFactor") config->aim_assist_pause_range_extension = stof(value);
	else if (name == "AimAssist::SpotDelay") config->aim_assist_spot_delay = stoi(value);
	else if (name == "AimAssist::ActivationSoftening") config->aim_assist_activation_softening = stoi(value);

	else if (name == "Triggerbot::On") config->triggerbot_on = String2Bool(value);
	else if (name == "Triggerbot::Key") config->triggerbot_key = ParseKey(value);
	else if (name == "Triggerbot::HoldKey") config->triggerbot_hold_key = String2Bool(value);
	else if (name == "Triggerbot::PlaySound") config->triggerbot_play_sound = String2Bool(value);
	else if (name == "Triggerbot::KeySpraydown") config->triggerbot_key_spraydown = ParseKey(value);
	else if (name == "Triggerbot::Mode") config->triggerbot_mode = stoi(value);
	else if (name == "Triggerbot::PauseAfterShot") config->triggerbot_pause_after_shot = stoi(value);
	else if (name == "Triggerbot::PauseRangeExtensionFactor") config->triggerbot_pause_range_extension_factor = stof(value);
	else if (name == "Triggerbot::FakeShotDuration") config->triggerbot_fake_shot_duration = stoi(value);
	else if (name == "Triggerbot::FireExtension") config->triggerbot_fire_extension = stoi(value);
	else if (name == "Triggerbot::Delay") config->triggerbot_delay = stoi(value);
	else if (name == "Triggerbot::MaximumDistance") config->triggerbot_maximum_distance = stof(value);
	else if (name == "Triggerbot::MaximumVelocity") config->triggerbot_maximum_velocity = stof(value);
	else if (name == "Triggerbot::DisableWhileEnteringScope") config->triggerbot_disable_while_entering_scope = String2Bool(value);
	else if (name == "Triggerbot::EnableInScopeOnly") config->triggerbot_enable_in_scope_only = String2Bool(value);
	else if (name == "Triggerbot::HeadOnly") config->triggerbot_head_only = String2Bool(value);
	else if (name == "Triggerbot::FriendlyFire") config->triggerbot_friendly_fire = String2Bool(value);
	else if (name == "Triggerbot::VisibilityCheck") config->triggerbot_visibility_check = String2Bool(value); 

	else if (name == "GlowESP::On") config->glow_esp_on = String2Bool(value);
	else if (name == "GlowESP::Key") config->glow_esp_key = ParseKey(value);
	else if (name == "GlowESP::HoldKey") config->glow_esp_hold_key = String2Bool(value);
	else if (name == "GlowESP::PlaySound") config->glow_esp_play_sound = String2Bool(value);
	else if (name == "GlowESP::ToggleForAllWeapons") config->glow_esp_toggle_for_all_weapons = String2Bool(value);
	else if (name == "GlowESP::SpottedOnly") config->glow_esp_spotted_only = String2Bool(value);
	else if (name == "GlowESP::DistinctColors") config->glow_esp_distinct_colors = String2Bool(value);
	else if (name == "GlowESP::DistinctMixFactor") config->glow_esp_distinct_mix_factor = stof(value);
	else if (name == "GlowESP::RGBA_Friendly") config->glow_esp_rgba_friendly = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Friendly1HP") config->glow_esp_rgba_friendly_1hp = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Hostile") config->glow_esp_rgba_hostile = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Hostile1HP") config->glow_esp_rgba_hostile_1hp = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Weapon") config->glow_esp_rgba_weapon = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Grenade") config->glow_esp_rgba_grenade = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Bomb") config->glow_esp_rgba_bomb = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_Money") config->glow_esp_rgba_money = std::stoll(value, nullptr, 16);
	else if (name == "GlowESP::RGBA_DistinctColors") config->distinct_colors.push_back(GlowData(std::stoll(value, nullptr, 16)));	 

	else if (name == "Aimbot::On") config->aimbot_on = String2Bool(value);
	else if (name == "Aimbot::Key") config->aimbot_key = ParseKey(value);
	else if (name == "Aimbot::HoldKey") config->aimbot_hold_key = String2Bool(value);
	else if (name == "Aimbot::PlaySound") config->aimbot_play_sound = String2Bool(value);
	else if (name == "Aimbot::FieldOfView") config->aimbot_field_of_view = stof(value);
	else if (name == "Aimbot::SingleShot") config->aimbot_single_shot = String2Bool(value);
	else if (name == "Aimbot::FireShotIfNoTargetFound") config->aimbot_fire_if_no_target = String2Bool(value);
	else if (name == "Aimbot::VisibilityCheck") config->aimbot_visibility_check = String2Bool(value);
	else if (name == "Aimbot::FriendlyFire") config->aimbot_friendly_fire = String2Bool(value);

	else if (name == "RCS::On") config->rcs_on = String2Bool(value);
	else if (name == "RCS::Key") config->rcs_key = ParseKey(value);
	else if (name == "RCS::HoldKey") config->rcs_hold_key = String2Bool(value);
	else if (name == "RCS::PlaySound") config->rcs_play_sound = String2Bool(value);
	else if (name == "RCS::ReductionFactor") config->rcs_reduction_factor = stof(value);
	else if (name == "RCS::SyncPause") config->rcs_sync_pause = stoi(value);

	config->updateConfigurationObjects();
}

void Parser::Distribute(vector<string> line, short run)
{
	if (line.size() == 2 && run == RUN_TYPES) for (Weapon& w : equipment.weapons) SetValue(w.GetDefaultConfig(), line.at(0), line.at(1));
	else if (line.size() == 3 && run == RUN_WEAPONS)
	{
		std::string specification = line.at(0);

		WeaponType weapon_type = Weapon::String2WeaponType(specification);
		WeaponName weapon_name = Weapon::String2WeaponName(specification);
		if (weapon_type != WeaponType::INVALID_TYPE) { for (Weapon& w : equipment.weapons) if (w.weapon_type == weapon_type) SetValue(w.GetDefaultConfig(), line.at(1), line.at(2)); }
		else if (weapon_name != WeaponName::INVALID_NAME) { for (Weapon& w : equipment.weapons) if (w.weapon_name == weapon_name) SetValue(w.GetDefaultConfig(), line.at(1), line.at(2)); }
	}
	else if (line.size() == 5 && run == RUN_VARIANT)
	{
		std::string specification = line.at(0);
		if (line.at(1) != "VARIANT_KEY") return;
		short key = ParseKey(line.at(2));

		WeaponType weapon_type = Weapon::String2WeaponType(specification);
		WeaponName weapon_name = Weapon::String2WeaponName(specification);
		if (weapon_type != WeaponType::INVALID_TYPE) { for (Weapon& w : equipment.weapons) if (w.weapon_type == weapon_type) SetValue(w.GetConfigFromKey(key), line.at(3), line.at(4)); }
		else if (weapon_name != WeaponName::INVALID_NAME) { for (Weapon& w : equipment.weapons) if (w.weapon_name == weapon_name) SetValue(w.GetConfigFromKey(key), line.at(3), line.at(4)); }
	}
	else if (line.size() == 4 && run == RUN_VARIANT)
	{
		if (line.at(0) != "VARIANT_KEY") return;
		short key = ParseKey(line.at(1));
		for (Weapon& w : equipment.weapons) SetValue(w.GetConfigFromKey(key), line.at(2), line.at(3));
	}
}

bool Parser::String2Bool(const string& s)
{
	return s.at(0) == '1';
}

bool Parser::ReadConfig()
{
	string path;
	GetConfigPath(&path, "config.cfg");

	if (!fs::exists(path))
	{
		Output::PrintTime();
		Output::PrintToConsole(path, 0, GRAY, BLACK);
		Output::PrintToConsole(" is missing", 1, WHITE, BLACK);
		Output::PrintToConsole("\nprovide the file and restart", 1, DARK_RED, BLACK);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		return false;
	}

	std::ifstream infile(path);

	for (Weapon& w : equipment.weapons) w.ResetAllConfigs();

	std::string line;
	for (short run = 0; run <= 2; run++)
	{
		while (std::getline(infile, line))
		{
			if (line.size() == 0 || line.at(0) == '#') continue;

			std::vector<std::string> words(0);
			words.push_back("");

			for (char& c : line)
			{
				if (c == ' ')
				{
					words.push_back("");
					continue;
				}
				words.back() += c;
			}
			Distribute(words, run);
		}
		infile.clear();
		infile.seekg(0, std::ios::beg);
	}

	infile.close();

	Output::PrintTime();
	Output::PrintToConsole("config.cfg ", GRAY, BLACK);
	Output::PrintToConsole("loaded", 1, WHITE, BLACK);

	return true;
}


bool Parser::LoadOffsets()
{
	string path;
	GetConfigPath(&path, "offsets.json");

	if (!fs::exists(path))
	{
		Output::PrintTime();
		Output::PrintToConsole(path, GRAY, BLACK);
		Output::PrintToConsole(" is missing", 1, WHITE, BLACK);
		Output::PrintToConsole("\nprovide the file and restart", 1, DARK_RED, BLACK);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		return false;
	}

	json data;

	std::ifstream infile(path);
	if (infile.peek() == std::ifstream::traits_type::eof())
	{
		Output::PrintTime();
		Output::PrintToConsole(path, GRAY, BLACK);
		Output::PrintToConsole(" is empty", 1, WHITE, BLACK);
		Output::PrintToConsole("\nprovide the correct file and restart", 1, DARK_RED, BLACK);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		return false;
	}

	std::ifstream offsets_file(path, std::ifstream::binary);
	offsets_file >> data;

	ptr_offset->dw_client_state = strtol(data["signatures"]["dwClientState"].dump().c_str(), 0, 0);
	ptr_offset->dw_client_state_is_hltv = strtol(data["signatures"]["dwClientState_IsHLTV"].dump().c_str(), 0, 0);
	ptr_offset->dw_client_state_state = strtol(data["signatures"]["dwClientState_State"].dump().c_str(), 0, 0);
	ptr_offset->dw_client_state_view_angles = strtol(data["signatures"]["dwClientState_ViewAngles"].dump().c_str(), 0, 0);
	ptr_offset->dw_entity_list = strtol(data["signatures"]["dwEntityList"].dump().c_str(), 0, 0);
	ptr_offset->dw_force_attack = strtol(data["signatures"]["dwForceAttack"].dump().c_str(), 0, 0);
	ptr_offset->dw_force_backward = strtol(data["signatures"]["dwForceBackward"].dump().c_str(), 0, 0);
	ptr_offset->dw_force_forward = strtol(data["signatures"]["dwForceForward"].dump().c_str(), 0, 0);
	ptr_offset->dw_force_jump = strtol(data["signatures"]["dwForceJump"].dump().c_str(), 0, 0);
	ptr_offset->dw_force_left = strtol(data["signatures"]["dwForceLeft"].dump().c_str(), 0, 0);
	ptr_offset->dw_force_right = strtol(data["signatures"]["dwForceRight"].dump().c_str(), 0, 0);
	ptr_offset->dw_glow_object_manager = strtol(data["signatures"]["dwGlowObjectManager"].dump().c_str(), 0, 0);
	ptr_offset->dw_local_player = strtol(data["signatures"]["dwLocalPlayer"].dump().c_str(), 0, 0);

	ptr_offset->armor_value = strtol(data["netvars"]["m_ArmorValue"].dump().c_str(), 0, 0);
	ptr_offset->aim_punch_angle = strtol(data["netvars"]["m_aimPunchAngle"].dump().c_str(), 0, 0);
	ptr_offset->aim_punch_angle_vel = strtol(data["netvars"]["m_aimPunchAngleVel"].dump().c_str(), 0, 0);
	ptr_offset->gun_game_immunity = strtol(data["netvars"]["m_bGunGameImmunity"].dump().c_str(), 0, 0);
	ptr_offset->in_reload = strtol(data["netvars"]["m_bInReload"].dump().c_str(), 0, 0);
	ptr_offset->is_scoped = strtol(data["netvars"]["m_bIsScoped"].dump().c_str(), 0, 0);
	ptr_offset->spotted_by_mask = strtol(data["netvars"]["m_bSpottedByMask"].dump().c_str(), 0, 0);
	ptr_offset->dw_bone_matrix = strtol(data["netvars"]["m_dwBoneMatrix"].dump().c_str(), 0, 0);
	ptr_offset->flags = strtol(data["netvars"]["m_fFlags"].dump().c_str(), 0, 0);
	ptr_offset->active_weapon = strtol(data["netvars"]["m_hActiveWeapon"].dump().c_str(), 0, 0);
	ptr_offset->clip = strtol(data["netvars"]["m_iClip1"].dump().c_str(), 0, 0);
	ptr_offset->crosshair_id = strtol(data["netvars"]["m_iCrosshairId"].dump().c_str(), 0, 0);
	ptr_offset->health = strtol(data["netvars"]["m_iHealth"].dump().c_str(), 0, 0);
	ptr_offset->item_definition_index = strtol(data["netvars"]["m_iItemDefinitionIndex"].dump().c_str(), 0, 0);
	ptr_offset->shots_fired = strtol(data["netvars"]["m_iShotsFired"].dump().c_str(), 0, 0);
	ptr_offset->team_num = strtol(data["netvars"]["m_iTeamNum"].dump().c_str(), 0, 0);
	ptr_offset->life_state = strtol(data["netvars"]["m_lifeState"].dump().c_str(), 0, 0);
	ptr_offset->vec_origin = strtol(data["netvars"]["m_vecOrigin"].dump().c_str(), 0, 0);
	ptr_offset->vec_velocity = strtol(data["netvars"]["m_vecVelocity"].dump().c_str(), 0, 0);
	ptr_offset->vec_view_offset = strtol(data["netvars"]["m_vecViewOffset"].dump().c_str(), 0, 0);


	time_t raw_time = std::stoi(data["timestamp"].dump());
	struct tm* timeinfo;
	char buffer[80];
#pragma warning(disable : 4996)
	timeinfo = localtime(&raw_time);
	strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", timeinfo);
	std::string dumped(buffer);

	Output::PrintTime();
	Output::PrintToConsole("offsets.json ", GRAY, BLACK);
	Output::PrintToConsole("loaded ", WHITE, BLACK);
	Output::PrintToConsole("/ timestamp " + dumped, 1, WHITE, BLACK);

	return true;
}