#include "memory.h"
#include "offsets.h"

int Memory::GetMaxEntity()
{
	int max_entity = *(int*)(client_handle + ptr_offset->dw_entity_list + 0x4);
	max_entity < 1024 ? max_entity : 1024;
	return max_entity;
}

bool Memory::GetValidEntity(int entity_id, DWORD* ptr_entity)
{
	*ptr_entity = *(DWORD*)(client_handle + ptr_offset->dw_entity_list + (entity_id - 1) * ptr_offset->entity_span);
	return *ptr_entity != 0;
}

bool Memory::GetLocalEntity(DWORD address)
{
	return address == *(DWORD*)(client_handle + ptr_offset->dw_local_player);
}

void Memory::GetEntityClassID(DWORD address, int* value)
{
	*value = *(int*)(*(DWORD*)(*(DWORD*)(*(DWORD*)(address + 0x8) + 0x8) + 0x1) + 0x14);
}

bool Memory::GetOnServer()
{
	return *(int*)(*(DWORD*)(engine_handle + ptr_offset->dw_client_state) + ptr_offset->dw_client_state_state) == 6;
}

void Memory::GetBoneMatrixVector(DWORD address, int boneId, Vector* vector)
{
	DWORD bone_matrix_base = *(DWORD*)(address + ptr_offset->dw_bone_matrix);
	if (bone_matrix_base != 0)
	{
		vector->x = *(float*)(bone_matrix_base + 0x0C + 0x30 * boneId);
		vector->y = *(float*)(bone_matrix_base + 0x0C + 0x30 * boneId + 0x10);
		vector->z = *(float*)(bone_matrix_base + 0x0C + 0x30 * boneId + 0x20);
	}
}

void Memory::GetEntityFromHandle(DWORD handle, DWORD* ptr_entity)
{
	*ptr_entity = *(DWORD*)(client_handle + ptr_offset->dw_entity_list + ((handle & 0xFFF) - 1) * 0x10);
}

void Memory::GetViewAngle(Axis* axis)
{
	*axis = *(Axis*)(*(DWORD*)(engine_handle + ptr_offset->dw_client_state) + ptr_offset->dw_client_state_view_angles);
}

void Memory::GetInHLTV(bool* in_hltv)
{
	*in_hltv = *reinterpret_cast<bool*>(*reinterpret_cast<DWORD*>(engine_handle + ptr_offset->dw_client_state) + ptr_offset->dw_client_state_is_hltv);
}

void Memory::SetAngle(Axis axis)
{
	*(Axis*)(*(DWORD*)(engine_handle + ptr_offset->dw_client_state) + ptr_offset->dw_client_state_view_angles) = axis;
}

void Memory::SubtractFromViewAngle(Axis axis)
{
	(*(Axis*)(*(DWORD*)(engine_handle + ptr_offset->dw_client_state) + ptr_offset->dw_client_state_view_angles)).SubtractDirect(axis);
}

void Memory::SetForceAttack(int i)
{
	*(int*)(client_handle + ptr_offset->dw_force_attack) = i;
}

// write complete struct?
void Memory::SetGlowData(DWORD glow_object, GlowData glow_data, bool render)
{
	if (glow_object == 0) return;
	*(float*)(glow_object + 0x8) = glow_data.red;
	*(float*)(glow_object + 0xC) = glow_data.green;
	*(float*)(glow_object + 0x10) = glow_data.blue;
	*(float*)(glow_object + 0x14) = glow_data.alpha;
	if (glow_data.alpha < 0.01f) *(bool*)(glow_object + 0x28) = false;
	else *(bool*)(glow_object + 0x28) = render;
}

int Memory::GetForceAttack()
{
	return *(int*)(client_handle + ptr_offset->dw_force_attack);
}

int Memory::GetGlowObjectManagerSize()
{
	return *(int*)(client_handle + ptr_offset->dw_glow_object_manager + 0xC);
}

DWORD Memory::GetGlowObject(int index)
{
	return *(DWORD*)(client_handle + ptr_offset->dw_glow_object_manager) + index * 0x38;
}