#include <thread>
#include <random>
#include <chrono>

#include "utility.h"
#include "control.h"
#include "equipment.h"
#include "parser.h"
#include "output.h"

DWORD client_handle = 0;
DWORD engine_handle = 0;

Offset* ptr_offset = nullptr;
Entity* ptr_local_entity = nullptr;
Entity* ptr_entity_list = nullptr;
Weapon* ptr_active_weapon = nullptr;
Configuration* ptr_active_config = nullptr;
Equipment equipment;

void main()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
#pragma warning( disable : 4996)
	freopen("CONOUT$", "w", stdout);

	Output::PrintToConsole("Anda's CS Cheat, 09/2019 version", 1, DARK_CYAN, BLACK);
	if (!Parser::ReadConfig()) return;

	if (!equipment.weapons.at(0).GetActiveConfig()->activate_console)
	{
		HWND stealth = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(stealth, 0);
	}

	Offset offset;
	ptr_offset = &offset;
	if (!Parser::LoadOffsets()) return;

	Output::PrintTime();
	Output::PrintToConsole("ACTIVE", 2, GREEN, BLACK);

	if (equipment.weapons.at(0).GetActiveConfig()->hide_console_after_startup)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		HWND stealth = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(stealth, 0);
	}

	while (client_handle == 0 || engine_handle == 0)
	{
		if ((DWORD)GetModuleHandle("client.dll") != 0) client_handle = (DWORD)GetModuleHandle("client.dll");
		if ((DWORD)GetModuleHandle("engine.dll") != 0) engine_handle = (DWORD)GetModuleHandle("engine.dll");
	}

	Entity list[1024];
	ptr_entity_list = list;
	Weapon active_weapon;
	ptr_active_weapon = &active_weapon;
	Control control;
	bool on_server = false;
	std::clock_t t_last_tick = std::clock();

	std::thread utility(&Control::SyncUtilityThread, control);

	for (;;)
	{
		if (Utility::ClockDiff(std::clock(), t_last_tick) < ptr_active_weapon->GetActiveConfig()->sync_break) continue;

		t_last_tick = std::clock();

		if (!Memory::GetOnServer())
		{
			if (on_server)
			{
				ptr_active_weapon = &active_weapon;
				control = Control();
				ptr_local_entity = nullptr;
			}
			on_server = false;
			continue;
		}

		on_server = true;

		int hostile_id = 0;
		for (int i = 0; i < Memory::GetMaxEntity(); i++) (ptr_entity_list + i)->Update(i, &hostile_id);
		if (ptr_local_entity == nullptr) continue;

		equipment.UpdateActiveWeapon();
		control.Sync();
	}
}

#pragma warning( disable : 4100)
BOOL WINAPI DllMain(HINSTANCE hdll, DWORD dw_reason, LPVOID)
{
	if (dw_reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}
	return TRUE;
}