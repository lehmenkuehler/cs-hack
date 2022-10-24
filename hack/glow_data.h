#pragma once

struct GlowData
{
	float red, green, blue, alpha;

	GlowData(long long color_hex)
	{
		red = ((color_hex >> 24) & 0xFF) / 255.f;
		green = ((color_hex >> 16) & 0xFF) / 255.f;
		blue = ((color_hex >> 8) & 0xFF) / 255.f;
		alpha = (color_hex & 0xFF) / 255.f;
	}
};