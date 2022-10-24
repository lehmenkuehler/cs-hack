#include "equipment.h"
#include "entity.h"

Equipment::Equipment()
{
	weapons.push_back(Weapon(DEAGLE, PISTOL, 1, 1100));
	weapons.push_back(Weapon(BERETTAS, PISTOL, 2, 1050));
	weapons.push_back(Weapon(FIVESEVEN, PISTOL, 3, 1000));
	weapons.push_back(Weapon(GLOCK18, PISTOL, 4, 1100));
	weapons.push_back(Weapon(AK47, ASSAULT_RIFLE, 7, 980));
	weapons.push_back(Weapon(AUG, ASSAULT_RIFLE, 8, 1170));
	weapons.push_back(Weapon(AWP, SNIPER_RIFLE, 9, 1250));
	weapons.push_back(Weapon(FAMAS, ASSAULT_RIFLE, 10, 1000));
	weapons.push_back(Weapon(G3SG1, SNIPER_RIFLE, 11, 1000));
	weapons.push_back(Weapon(GALILAR, ASSAULT_RIFLE, 13, 1110));
	weapons.push_back(Weapon(M249, MACHINE_GUN, 14, 1130));
	weapons.push_back(Weapon(M4A4, ASSAULT_RIFLE, 16, 1110));
	weapons.push_back(Weapon(MAC10, SUBMACHINE_GUN, 17, 980));
	weapons.push_back(Weapon(P90, SUBMACHINE_GUN, 19, 930));
	weapons.push_back(Weapon(MP5SD, SUBMACHINE_GUN, 23, 1100));
	weapons.push_back(Weapon(UMP45, SUBMACHINE_GUN, 24, 1030));
	weapons.push_back(Weapon(XM1014, SHOTGUN, 25, 970));
	weapons.push_back(Weapon(BIZON, SUBMACHINE_GUN, 26, 1100));
	weapons.push_back(Weapon(MAG7, SHOTGUN, 27, 930));
	weapons.push_back(Weapon(NEGEV, MACHINE_GUN, 28, 1080));
	weapons.push_back(Weapon(SAWEDOFF, SHOTGUN, 29, 1070));
	weapons.push_back(Weapon(TEC9, PISTOL, 30, 970));
	weapons.push_back(Weapon(TASER, PISTOL, 31, 0));
	weapons.push_back(Weapon(P2000, PISTOL, 32, 970));
	weapons.push_back(Weapon(MP7, SUBMACHINE_GUN, 33, 1000));
	weapons.push_back(Weapon(MP9, SUBMACHINE_GUN, 34, 1280));
	weapons.push_back(Weapon(NOVA, SHOTGUN, 35, 1020));
	weapons.push_back(Weapon(P250, PISTOL, 36, 970));
	weapons.push_back(Weapon(SCAR20, SNIPER_RIFLE, 38, 1000));
	weapons.push_back(Weapon(SG553, ASSAULT_RIFLE, 39, 950));
	weapons.push_back(Weapon(SSG08, SNIPER_RIFLE, 40, 970));
	weapons.push_back(Weapon(KNIFE_CT, KNIFE, 42, 0));
	weapons.push_back(Weapon(FLASHBANG, GRENADE, 43, 0));
	weapons.push_back(Weapon(HEGRENADE, GRENADE, 44, 0));
	weapons.push_back(Weapon(SMOKEGRENADE, GRENADE, 45, 0));
	weapons.push_back(Weapon(MOLOTOV, GRENADE, 46, 0));
	weapons.push_back(Weapon(DECOY, GRENADE, 47, 0));
	weapons.push_back(Weapon(INCGRENADE, GRENADE, 48, 0));
	weapons.push_back(Weapon(C4, BOMB, 49, 0));
	weapons.push_back(Weapon(HEALTHSHOT, UTILITY, 57, 0));
	weapons.push_back(Weapon(KNIFE_T, KNIFE, 59, 0));
	weapons.push_back(Weapon(M4A1S, ASSAULT_RIFLE, 60, 1110));
	weapons.push_back(Weapon(USPS, PISTOL, 61, 980));
	weapons.push_back(Weapon(CZ75A, PISTOL, 63, 1830));
	weapons.push_back(Weapon(REVOLVER, PISTOL, 64, 1300));
}

void Equipment::UpdateActiveWeapon()
{
	for (unsigned int k = 0; k < weapons.size(); k++)
	{
		if (weapons.at(k).item_index == ptr_local_entity->active_weapon_item_index)
		{
			ptr_active_weapon = &weapons[k];
			return;
		}
	}
}