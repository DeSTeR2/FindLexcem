#pragma once
#include <algorithm>
#include <cmath>

struct Color {
	float R;
	float G;
	float B;

	Color();
	Color(float r, float g, float b);
	void toRGBInt(int& r_out, int& g_out, int& b_out) const;
};