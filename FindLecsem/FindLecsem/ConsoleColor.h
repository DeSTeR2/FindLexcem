#pragma once
#include <string>
#include "Color.h"

namespace ConsoleColor {
    bool enableVirtualTerminal();

    std::string ansiFg(const Color& c);

    std::string ansiBg(const Color& c);

    void print(const std::string& text, const Color& fg, bool newline = false);

    inline std::string reset() { return "\x1b[0m"; }
}
