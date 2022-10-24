#pragma once
#include <Windows.h>

#include "axis.h"
#include "vector.h"
#include "configuration.h"

extern DWORD client_handle;
extern DWORD engine_handle;

namespace Memory
{
	int GetMaxEntity();
	bool GetValidEntity(int entity_id, DWORD* ptr_entity);
	bool GetLocalEntity(DWORD address);
	void GetEntityClassID(DWORD address, int* value);
	bool GetOnServer();

	template<typename T>
	void Get(DWORD address, const ptrdiff_t offset, T* value);

	void GetBoneMatrixVector(DWORD address, int bone_id, Vector* vector);
	void GetEntityFromHandle(DWORD handle, DWORD* ptr_entity);
	void GetViewAngle(Axis* axis);
	void GetInHLTV(bool* in_hltv);


	int GetForceAttack();
	int GetGlowObjectManagerSize();

	template<typename T>
	void Set(DWORD address, const ptrdiff_t offset, T value);

	void SetAngle(Axis axis);
	void SubtractFromViewAngle(Axis axis);
	void SetForceAttack(int i);
	void SetGlowData(DWORD ptr_glow_object, GlowData glow_data, bool render);

	DWORD GetGlowObject(int index);
};

template<typename T>
inline void Memory::Get(DWORD address, const ptrdiff_t offset, T* value)
{
	if (address != 0) *value = *(T*)(address + offset);
}

template<typename T>
inline void Memory::Set(DWORD address, const ptrdiff_t offset, T value)
{
	if (address == 0) return;
	*(T*)(address + offset) = value;
}