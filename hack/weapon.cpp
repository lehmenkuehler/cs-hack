#include "weapon.h"

Weapon::Weapon()
{
	weapon_name = INVALID_NAME;
	weapon_type = INVALID_TYPE;
	item_index = 0;
	configs.push_back(Configuration());
	time = 0;
};

Weapon::Weapon(WeaponName name, WeaponType type, short index, int t_switch)
{
	weapon_name = name;
	weapon_type = type;
	item_index = index;
	time = t_switch;
	configs.push_back(Configuration());
};

WeaponName Weapon::String2WeaponName(string name)
{
	if (name == "DEAGLE") return WeaponName::DEAGLE;
	else if (name == "BERETTAS") return WeaponName::BERETTAS;
	else if (name == "FIVESEVEN") return WeaponName::FIVESEVEN;
	else if (name == "GLOCK18") return WeaponName::GLOCK18;
	else if (name == "AK47") return WeaponName::AK47;
	else if (name == "AUG") return WeaponName::AUG;
	else if (name == "AWP") return WeaponName::AWP;
	else if (name == "FAMAS") return WeaponName::FAMAS;
	else if (name == "G3SG1") return WeaponName::G3SG1;
	else if (name == "GALILAR") return WeaponName::GALILAR;
	else if (name == "M249") return WeaponName::M249;
	else if (name == "M4A4") return WeaponName::M4A4;
	else if (name == "MAC10") return WeaponName::MAC10;
	else if (name == "P90") return WeaponName::P90;
	else if (name == "MP5SD") return WeaponName::MP5SD;
	else if (name == "UMP45") return WeaponName::UMP45;
	else if (name == "XM1014") return WeaponName::XM1014;
	else if (name == "BIZON") return WeaponName::BIZON;
	else if (name == "MAG7") return WeaponName::MAG7;
	else if (name == "NEGEV") return WeaponName::NEGEV;
	else if (name == "SAWEDOFF") return WeaponName::SAWEDOFF;
	else if (name == "TEC9") return WeaponName::TEC9;
	else if (name == "TASER") return WeaponName::TASER;
	else if (name == "P2000") return WeaponName::P2000;
	else if (name == "MP7") return WeaponName::MP7;
	else if (name == "MP9") return WeaponName::MP9;
	else if (name == "NOVA") return WeaponName::NOVA;
	else if (name == "P250") return WeaponName::P250;
	else if (name == "SCAR20") return WeaponName::SCAR20;
	else if (name == "SG553") return WeaponName::SG553;
	else if (name == "SSG08") return WeaponName::SSG08;
	else if (name == "KNIFE_CT") return WeaponName::KNIFE_CT;
	else if (name == "FLASHBANG") return WeaponName::FLASHBANG;
	else if (name == "HEGRENADE") return WeaponName::HEGRENADE;
	else if (name == "SMOKEGRENADE") return WeaponName::SMOKEGRENADE;
	else if (name == "MOLOTOV") return WeaponName::MOLOTOV;
	else if (name == "DECOY") return WeaponName::DECOY;
	else if (name == "INCGRENADE") return WeaponName::INCGRENADE;
	else if (name == "C4") return WeaponName::C4;
	else if (name == "KNIFE_T") return WeaponName::KNIFE_T;
	else if (name == "M4A1S") return WeaponName::M4A1S;
	else if (name == "USPS") return WeaponName::USPS;
	else if (name == "CZ75A") return WeaponName::CZ75A;
	else if (name == "REVOLVER") return WeaponName::REVOLVER;
	else if (name == "KNIFE_BAYONET") return WeaponName::KNIFE_BAYONET;
	else if (name == "KNIFE_FLIP") return WeaponName::KNIFE_FLIP;
	else if (name == "KNIFE_GUT") return WeaponName::KNIFE_GUT;
	else if (name == "KNIFE_KARAMBIT") return WeaponName::KNIFE_KARAMBIT;
	else if (name == "KNIFE_M9_BAYONET") return WeaponName::KNIFE_M9_BAYONET;
	else if (name == "KNIFE_TACTICAL") return WeaponName::KNIFE_TACTICAL;
	else if (name == "KNIFE_FALCHION") return WeaponName::KNIFE_FALCHION;
	else if (name == "KNIFE_SURVIVAL_BOWIE") return WeaponName::KNIFE_SURVIVAL_BOWIE;
	else if (name == "KNIFE_BUTTERFLY") return WeaponName::KNIFE_BUTTERFLY;
	else if (name == "KNIFE_PUSH") return WeaponName::KNIFE_PUSH;
	else return WeaponName::INVALID_NAME;
}

WeaponType Weapon::String2WeaponType(string type)
{
	if (type == "KNIFE") return WeaponType::KNIFE;
	else if (type == "PISTOL") return WeaponType::PISTOL;
	else if (type == "SHOTGUN") return WeaponType::SHOTGUN;
	else if (type == "MACHINE_GUN") return WeaponType::MACHINE_GUN;
	else if (type == "SUBMACHINE_GUN") return WeaponType::SUBMACHINE_GUN;
	else if (type == "ASSAULT_RIFLE") return WeaponType::ASSAULT_RIFLE;
	else if (type == "SNIPER_RIFLE") return WeaponType::SNIPER_RIFLE;
	else if (type == "GRENADE") return WeaponType::GRENADE;
	else if (type == "BOMB") return WeaponType::BOMB;
	else return WeaponType::INVALID_TYPE;
}

Configuration* Weapon::GetActiveConfig()
{
	for (Configuration& c : configs) if (c.button_variant.is_active()) return &c;
	return &configs.at(0);
}

Configuration* Weapon::GetDefaultConfig()
{
	return &configs.at(0);
}

Configuration* Weapon::GetConfigFromKey(short key)
{
	for (Configuration& c : configs) if (c.button_variant.key_code == key) return &c;
	configs.push_back(configs.at(0));
	configs.back().variant_key = key;
	return &configs.back();
}

void Weapon::ResetAllConfigs()
{
	for (unsigned int i = 1; i < configs.size(); i++) configs.pop_back();
}