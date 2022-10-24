#pragma once
#include "memory.h"
#include "entity.h"
#include "configuration.h"

struct BufferElement
{
	BufferElement()
	{
		ptr_entity = 0;
		color_id = -1;
	}

	BufferElement(DWORD player_entity, int color)
	{
		ptr_entity = player_entity;
		color_id = color;
	}

	DWORD ptr_entity;
	int color_id;
};

class Glow
{
public:
	Glow();
	void Activate();
	void Reset();
private:
	int GetIdType(int classId);

	std::vector<BufferElement> buffer;
	void ProcessEntity(DWORD ptr_entity, int* color);
};