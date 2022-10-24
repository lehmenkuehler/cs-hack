#pragma once
#include <string>
#include <vector>

#include "configuration.h"

using std::string;
using std::vector;

enum WeaponType
{
	INVALID_TYPE,
	KNIFE,
	PISTOL,
	SHOTGUN,
	MACHINE_GUN,
	SUBMACHINE_GUN,
	ASSAULT_RIFLE,
	SNIPER_RIFLE,
	GRENADE,
	BOMB,
	UTILITY
};

enum WeaponName
{
	INVALID_NAME,
	DEAGLE,
	BERETTAS,
	FIVESEVEN,
	GLOCK18,
	AK47,
	AUG,
	AWP,
	FAMAS,
	G3SG1,
	GALILAR,
	M249,
	M4A4,
	MAC10,
	P90,
	MP5SD,
	UMP45,
	XM1014,
	BIZON,
	MAG7,
	NEGEV,
	SAWEDOFF,
	TEC9,
	TASER,
	P2000,
	MP7,
	MP9,
	NOVA,
	P250,
	SCAR20,
	SG553,
	SSG08,
	KNIFE_CT,
	FLASHBANG,
	HEGRENADE,
	SMOKEGRENADE,
	MOLOTOV,
	DECOY,
	INCGRENADE,
	C4,
	KNIFE_T,
	M4A1S,
	USPS,
	CZ75A,
	REVOLVER,
	KNIFE_BAYONET,
	KNIFE_FLIP,
	KNIFE_GUT,
	KNIFE_KARAMBIT,
	KNIFE_M9_BAYONET,
	KNIFE_TACTICAL,
	KNIFE_FALCHION,
	KNIFE_SURVIVAL_BOWIE,
	KNIFE_BUTTERFLY,
	KNIFE_PUSH,
	HEALTHSHOT
};

struct Weapon
{
	Weapon();
	Weapon(WeaponName name, WeaponType type, short index, int switch_time);
	static WeaponName String2WeaponName(string name);
	static WeaponType String2WeaponType(string type);
	Configuration* GetActiveConfig();
	Configuration* GetDefaultConfig();
	Configuration* GetConfigFromKey(short key);
	void ResetAllConfigs();

	WeaponName weapon_name;
	WeaponType weapon_type;
	short item_index;
	int time;
	vector <Configuration> configs;
};

extern Weapon* ptr_active_weapon;