#pragma once
#include <Windows.h>

struct Button
{
	int key_code;
	bool last_tick_state, currently_pressed;
	bool active;
	bool hold;

	Button(int key_code, bool hold, bool active_from_start)
	{
		this->key_code = key_code;
		this->hold = hold;
		active = active_from_start;
		last_tick_state = false;
		currently_pressed = false;
	}

	void update(bool play_sound)
	{
		if (key_code == -1) return;
		currently_pressed = GetAsyncKeyState(key_code);
		if (hold)
		{
			active = currently_pressed;
			if (play_sound)
			{
				if (!last_tick_state && active) Beep(600, 200);
				else if (last_tick_state && !active) Beep(400, 200);
			}
		}
		else
		{
			if (currently_pressed && last_tick_state != currently_pressed)
			{
				active = !active;
				if (play_sound)
				{
					if (active) Beep(600, 200);
					else Beep(400, 200);
				}
			}
		}
		last_tick_state = currently_pressed;
	}

	void set_status(bool is_active)
	{
		active = is_active;
	}

	bool is_active()
	{
		return active;
	}
};