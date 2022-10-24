#pragma once
#include "weapon.h"

struct Equipment
{
	Equipment();
	void UpdateActiveWeapon();
	std::vector<Weapon> weapons;
};

extern Equipment equipment;