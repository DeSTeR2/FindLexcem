#include "ConsoleColor.h"
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

bool ConsoleColor::enableVirtualTerminal() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return false;
    const DWORD ENABLE = 0x0004;
    if ((dwMode & ENABLE) == 0) {
        if (!SetConsoleMode(hOut, dwMode | ENABLE)) {
            return false;
        }
    }
    return true;
}

static std::string makeAnsi(bool foreground, int r, int g, int b) {
    std::ostringstream ss;
    if (foreground) {
        ss << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
    }
    else {
        ss << "\x1b[48;2;" << r << ";" << g << ";" << b << "m";
    }
    return ss.str();
}

std::string ConsoleColor::ansiFg(const Color& c) {
    int r, g, b;
    c.toRGBInt(r, g, b);
    return makeAnsi(true, r, g, b);
}

std::string ConsoleColor::ansiBg(const Color& c) {
    int r, g, b;
    c.toRGBInt(r, g, b);
    return makeAnsi(false, r, g, b);
}

void ConsoleColor::print(const std::string& text, const Color& fg, bool newline) {
    enableVirtualTerminal();

    std::string seq = ansiFg(fg);

    seq += text;
    seq += reset();
    if (newline) 
        seq += '\n';
    fwrite(seq.c_str(), 1, seq.size(), stdout);
}
