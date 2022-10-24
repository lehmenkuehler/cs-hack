#pragma once
#include "aim_assist.h"
#include "aimbot.h"
#include "recoil_control.h"
#include "triggerbot.h"
#include "glow.h"

class Control
{
public:
	Control();
	void Sync();
	void SyncUtilityThread();
private:
	void SyncAimbot();
	void SyncAimAssist();
	void SyncRCS();
	void SyncGlowESP();
	void SyncTriggerbot();

	void LoadConfig();
	void SyncPanic();

	void InitializeControl();

	short weapon_item_index_last_tick;

	AimAssist aim_assist;
	Aimbot aimbot;
	RecoilControl recoil_reduction;
	Triggerbot trigger_bot;
	Glow glow;

	std::clock_t t_last_config_update;
};