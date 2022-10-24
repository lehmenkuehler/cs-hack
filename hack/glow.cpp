#include <bitset>

#include "glow.h"
#include "offsets.h"
#include "weapon.h"
#include "utility.h"

constexpr short C_INVALID = 0;
constexpr short C_PLAYER = 1;
constexpr short C_WEAPON = 2;
constexpr short C_GRENADE = 3;
constexpr short C_OBJECTIVE = 4;
constexpr short C_MONEY = 6;

Glow::Glow()
{
}

void Glow::Activate()
{

	//	int32_t m_nNextFreeSlot; 0x0000
	//	uint32_t pEntity; 0x0004
	//	float R; 0x0008
	//	float G; 0x000C
	//	float B; 0x0010
	//	float A; 0x0014
	//	char pad_0018[4]; 0x0018
	//	float flUnk; 0x001C
	//	float flBloom; 0x0020
	//	float localplayeriszeropoint3; 0x0024
	//	bool m_bRenderWhenOccluded; 0x0028
	//	bool m_bRenderWhenUnoccluded; 0x0029
	//	bool m_bFullBloomRender; 0x002A
	//	char pad_002B[1]; 0x002B
	//	int32_t m_nFullBloomStencilTestValue; 0x002C
	//	int32_t iUnk; 0x0030
	//	int32_t m_nSplitScreenSlot; 0x0034
	//  size: 0x0038

	if (ptr_local_entity == nullptr) return;
	int object_count = Memory::GetGlowObjectManagerSize();

	for (int k = 0; k < object_count; k++)
	{
		DWORD ptr_glow_object = Memory::GetGlowObject(k);

		DWORD player_entity = *(DWORD*)(ptr_glow_object + 0x4);
		if (player_entity == 0) continue;

		int class_id = -1;
		Memory::GetEntityClassID(player_entity, &class_id);

		if (GetIdType(class_id) == C_PLAYER)
		{
			if (ptr_active_weapon->GetActiveConfig()->glow_esp_spotted_only && ptr_local_entity->life_state != LIFE_STATE_DEAD)
			{
				int spotted_by;
				Memory::Get(player_entity, ptr_offset->spotted_by_mask, &spotted_by);
				std::bitset<32> bits(spotted_by);
				if (bits[ptr_local_entity->id - 1] == 0) continue;
			}

			int team;
			Memory::Get(player_entity, ptr_offset->team_num, &team);

			GlowData mixed_glow(0x0), glow_100hp(0x0), glow_1hp(0x0);

			if (team == ptr_local_entity->team)
			{
				glow_100hp = ptr_active_weapon->GetActiveConfig()->glow_team;
				glow_1hp = ptr_active_weapon->GetActiveConfig()->glow_team_1hp;
			}
			else
			{
				glow_100hp = ptr_active_weapon->GetActiveConfig()->glow_hostile;
				glow_1hp = ptr_active_weapon->GetActiveConfig()->glow_hostile_1hp;
			}

			int health;
			Memory::Get(player_entity, ptr_offset->health, &health);
			float h = static_cast<float>(health) / 100.f;

			int color;
			ProcessEntity(player_entity, &color);

			if (team != ptr_local_entity->team && ptr_active_weapon->GetActiveConfig()->glow_esp_distinct_colors && ptr_active_weapon->GetActiveConfig()->distinct_colors.size() > 0)
			{
				h = (1.f - ptr_active_weapon->GetActiveConfig()->glow_esp_distinct_mix_factor) * h + ptr_active_weapon->GetActiveConfig()->glow_esp_distinct_mix_factor;
				mixed_glow.red = ptr_active_weapon->GetActiveConfig()->distinct_colors.at(color).red * h + glow_1hp.red * (1.f - h);
				mixed_glow.green = ptr_active_weapon->GetActiveConfig()->distinct_colors.at(color).green * h + glow_1hp.green * (1.f - h);
				mixed_glow.blue = ptr_active_weapon->GetActiveConfig()->distinct_colors.at(color).blue * h + glow_1hp.blue * (1.f - h);
				mixed_glow.alpha = ptr_active_weapon->GetActiveConfig()->distinct_colors.at(color).alpha * h + glow_1hp.alpha * (1.f - h);
			}
			else
			{
				mixed_glow.red = glow_100hp.red * h + glow_1hp.red * (1.f - h);
				mixed_glow.green = glow_100hp.green * h + glow_1hp.green * (1.f - h);
				mixed_glow.blue = glow_100hp.blue * h + glow_1hp.blue * (1.f - h);
				mixed_glow.alpha = glow_100hp.alpha * h + glow_1hp.alpha * (1.f - h);
			}

			Memory::SetGlowData(ptr_glow_object, mixed_glow, true);

		}
		else if (GetIdType(class_id) == C_WEAPON) Memory::SetGlowData(ptr_glow_object, ptr_active_weapon->GetActiveConfig()->glow_weapon, true);
		else if (GetIdType(class_id) == C_GRENADE) Memory::SetGlowData(ptr_glow_object, ptr_active_weapon->GetActiveConfig()->glow_grenade, true);
		else if (GetIdType(class_id) == C_OBJECTIVE) Memory::SetGlowData(ptr_glow_object, ptr_active_weapon->GetActiveConfig()->glow_bomb, true);
		else if (GetIdType(class_id) == C_MONEY) Memory::SetGlowData(ptr_glow_object, GlowData(0x00FF00FF), true);
	}
}

void Glow::Reset()
{
	if (ptr_local_entity == 0) return;
	int object_count = Memory::GetGlowObjectManagerSize();
	GlowData standard = GlowData(0x00000000);

	for (int k = 0; k < object_count; k++)
	{
		DWORD glow_object = Memory::GetGlowObject(k);

		DWORD player_entity = *(DWORD*)(glow_object + 0x4);
		if (player_entity == 0) continue;

		int class_id = -1;
		Memory::GetEntityClassID(player_entity, &class_id);

		if (GetIdType(class_id) == C_PLAYER ||
			GetIdType(class_id) == C_WEAPON ||
			GetIdType(class_id) == C_GRENADE ||
			GetIdType(class_id) == C_OBJECTIVE) Memory::SetGlowData(glow_object, standard, true);
	}

	buffer.clear();
}

int Glow::GetIdType(int classId)
{
	// sv_dump_class_info
	if (classId == 40) return C_PLAYER;
	// AK47, Deagle, Rest
	else if (classId == 1 || classId == 46 || (classId >= 231 && classId <= 272)) return C_WEAPON;
	// decoy, flashbang, high explosive, incendiary, molotov, smoke
	else if (classId == 47 || classId == 77 || classId == 96 || classId == 99 || classId == 112 || classId == 155) return GRENADE;
	// bomb, bomb planted, hostage
	else if (classId == 34 || classId == 128 || classId == 97) return C_OBJECTIVE;
	else if (classId == 105) return C_MONEY;
	else return C_INVALID;
}

void Glow::ProcessEntity(DWORD player_entity, int* color)
{
	for (unsigned int i = 0; i < buffer.size(); i++)
	{
		if (buffer.at(i).ptr_entity == player_entity)
		{
			*color = buffer.at(i).color_id;
			return;
		}
	}
	unsigned int color_id;
	if (buffer.size() != 0) color_id = buffer.back().color_id + 1;
	else color_id = 0;
	if (color_id == ptr_active_weapon->GetActiveConfig()->distinct_colors.size()) color_id = 0;
	buffer.push_back(BufferElement(player_entity, color_id));
	*color = color_id;
}