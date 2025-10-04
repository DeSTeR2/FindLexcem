#include "Color.h"

Color::Color() {
	R = 1;
	G = 1;
	B = 1;
}

Color::Color(float r, float g, float b) {
	R = r;
	G = g;
	B = b;
}

void Color::toRGBInt(int& r_out, int& g_out, int& b_out) const {
    auto conv = [](float v)->int {
        if (v <= 1.0f && v >= 0.0f) {
            v = std::max(0.0f, std::min(1.0f, v));
            return static_cast<int>(std::round(v * 255.0f));
        }
        else {
            v = std::max(0.0f, std::min(255.0f, v));
            return static_cast<int>(std::round(v));
        }
        };
    r_out = conv(R);
    g_out = conv(G);
    b_out = conv(B);
}
