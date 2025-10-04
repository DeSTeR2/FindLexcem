// simple_test.cpp

#include <iostream>
#include <vector>
#include <string>

#define THRESHOLD 5

// single-line comment
/* multi-line
   comment example */

struct Point {
    int x;
    int y;
};

int add(int a, int b) {
    return a + b;
}

int main() {
    int n = 10;
    unsigned long long total = 1'000'000ULL; // numeric separator
    int oct = 012;        // octal (value 10)
    int bin = 0b1010;     // binary (value 10)
    int hexVal = 0xFF;    // hex
    double pi = 3.14159;
    double small = .25;

    char ch = 'A';
    const char* s = "Hello, C++";
    const char* concat = "Hello" + "Concat"; // adjacent strings

#if THRESHOLD > 3
    std::cout << "Threshold high\n";
#endif

    std::vector<Point> pts;
    pts.push_back({ 1, 2 });
    pts.push_back({ 3, 4 });

    for (int i = 0; i < n; ++i) {
        int v = add(i, bin);
        if (v % 2 == 0) {
            total += v;
        }
        else {
            total -= v;
        }
    }

    std::cout << "total=" << total << ", pi=" << pi << ", small=" << small << '\n';
    std::cout << "hex=" << hexVal << ", ch=" << ch << ", s=" << s << '\n';
    std::cout << "concat: " << concat << '\n';

    // raw string example (simple)
    const char* raw = "(Line1 Line2)";
    std::cout << raw << '\n';

    return 0;
}