#include <chrono>
#include <thread>

#include "equipment.h"
#include "weapon.h"
#include "control.h"
#include "glow.h"
#include "parser.h"
#include "utility.h"

Control::Control()
{
	weapon_item_index_last_tick = 0;
	InitializeControl();
}

void Control::Sync()
{
	if (weapon_item_index_last_tick != ptr_active_weapon->item_index) InitializeControl();

	for (Configuration& c : ptr_active_weapon->configs) c.button_variant.update(c.variant_play_sound);

	SyncAimbot();
	SyncAimAssist();
	SyncRCS();
	SyncGlowESP();
	SyncTriggerbot();

	weapon_item_index_last_tick = ptr_active_weapon->item_index;
}

void Control::SyncUtilityThread()
{
	while (true)
	{
		LoadConfig();
		SyncPanic();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Control::InitializeControl()
{
	aim_assist = AimAssist();
	recoil_reduction = RecoilControl();
	trigger_bot = Triggerbot();
	glow = Glow();
	aimbot = Aimbot();
}

void Control::SyncAimAssist()
{
	if (!ptr_active_weapon->GetActiveConfig()->aim_assist_on) return;
	ptr_active_weapon->GetActiveConfig()->button_aim_assist.update(ptr_active_weapon->GetActiveConfig()->aim_assist_play_sound);
	if (ptr_active_weapon->GetActiveConfig()->button_aim_assist.is_active()) aim_assist.Activate();
	else aim_assist.Reset();
}

void Control::SyncRCS()
{
	if (!ptr_active_weapon->GetActiveConfig()->rcs_on) return;
	ptr_active_weapon->GetActiveConfig()->button_rcs.update(ptr_active_weapon->GetActiveConfig()->rcs_play_sound);
	if (ptr_active_weapon->GetActiveConfig()->button_rcs.is_active()) recoil_reduction.Activate();
	else recoil_reduction.Reset();
}

void Control::SyncGlowESP()
{
	if (!ptr_active_weapon->GetActiveConfig()->glow_esp_on) return;
	ptr_active_weapon->GetActiveConfig()->button_glow_esp.update(ptr_active_weapon->GetActiveConfig()->glow_esp_play_sound);
	if (ptr_active_weapon->GetActiveConfig()->glow_esp_toggle_for_all_weapons) for (Weapon& weapon : equipment.weapons) for (Configuration& c : weapon.configs) c.button_glow_esp.set_status(ptr_active_weapon->GetActiveConfig()->button_glow_esp.is_active());
	if (ptr_active_weapon->GetActiveConfig()->button_glow_esp.is_active()) glow.Activate();
	else glow.Reset();
}

void Control::SyncTriggerbot()
{
	if (!ptr_active_weapon->GetActiveConfig()->triggerbot_on) return;
	ptr_active_weapon->GetActiveConfig()->button_triggerbot.update(ptr_active_weapon->GetActiveConfig()->triggerbot_play_sound);
	ptr_active_weapon->GetActiveConfig()->button_triggerbot_spraydown.update(ptr_active_weapon->GetActiveConfig()->triggerbot_play_sound);
	if (ptr_active_weapon->GetActiveConfig()->button_triggerbot.is_active()) trigger_bot.Activate(false);
	else if (ptr_active_weapon->GetActiveConfig()->button_triggerbot_spraydown.is_active()) trigger_bot.Activate(true);
}

void Control::SyncAimbot()
{
	if (!ptr_active_weapon->GetActiveConfig()->aimbot_on) return;
	ptr_active_weapon->GetActiveConfig()->button_aimbot.update(ptr_active_weapon->GetActiveConfig()->aimbot_play_sound);
	if (ptr_active_weapon->GetActiveConfig()->button_aimbot.is_active()) aimbot.Update();
	else aimbot.SetInactive();
}

void Control::LoadConfig()
{
	ptr_active_weapon->GetActiveConfig()->button_load_config.update(false);
	if (ptr_active_weapon->GetActiveConfig()->button_load_config.is_active() && Utility::ClockDiff(std::clock(), t_last_config_update) > 3000)
	{
		Beep(400, 200);
		Parser::ReadConfig();
		glow.Reset();
		InitializeControl();
		t_last_config_update = std::clock();
	}
}

void Control::SyncPanic()
{
	ptr_active_weapon->GetActiveConfig()->button_panic.update(false);
	if (ptr_active_weapon->GetActiveConfig()->button_panic.is_active()) exit(0);
}