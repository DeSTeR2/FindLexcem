#include "ConfigParser.h"
#include "ResourceProvider.h"
#include "LexcemColorData.h"
#include "LexcemDataType.h"
#include "Color.h"

#include <string>
#include <memory>
#include <vector>
#include <cctype>
#include <iostream>
#include <sstream>

// Helper: skip whitespace
static void skip_ws(const std::string& s, size_t& pos) {
    while (pos < s.size() && std::isspace(s[pos])) ++pos;
}

// Helper: parse JSON string (handles escaped quotes)
static bool parse_json_string(const std::string& s, size_t& pos, std::string& out) {
    if (s[pos] != '"') return false;
    ++pos;
    out.clear();
    while (pos < s.size()) {
        char c = s[pos++];
        if (c == '\\' && pos < s.size()) {
            char n = s[pos++];
            switch (n) {
            case '"': out.push_back('"'); break;
            case '\\': out.push_back('\\'); break;
            case '/': out.push_back('/'); break;
            case 'b': out.push_back('\b'); break;
            case 'f': out.push_back('\f'); break;
            case 'n': out.push_back('\n'); break;
            case 'r': out.push_back('\r'); break;
            case 't': out.push_back('\t'); break;
            case 'u':
                if (pos + 4 <= s.size()) {
                    out.append(s.substr(pos - 1, 5));
                    pos += 4;
                }
                break;
            default: out.push_back(n); break;
            }
        }
        else if (c == '"') {
            return true; // end of string
        }
        else {
            out.push_back(c);
        }
    }
    return false; // unmatched quote
}

// Helper: parse key:value pair like "R":255
static bool parse_color_component(const std::string& s, size_t& pos, char& key, float& val) {
    skip_ws(s, pos);
    if (pos >= s.size()) return false;
    // parse key (R,G,B)
    if (s[pos] == '\'' || std::isalpha(s[pos])) {
        if (s[pos] == '\'') ++pos;
        key = s[pos++];
        if (s[pos] == '\'') ++pos;
        skip_ws(s, pos);
        if (pos >= s.size() || s[pos] != ':') return false;
        ++pos;
        skip_ws(s, pos);
        size_t start = pos;
        while (pos < s.size() && (std::isdigit(s[pos]) || s[pos] == '.' || s[pos] == 'e' || s[pos] == 'E' || s[pos] == '+' || s[pos] == '-')) ++pos;
        val = std::stof(s.substr(start, pos - start));
        return true;
    }
    return false;
}

static std::vector<std::string> extract_top_level_objects(const std::string& s) {
    std::vector<std::string> objs;
    size_t pos = 0;
    while (pos < s.size() && s[pos] != '[') ++pos;
    if (pos == s.size()) return objs;
    ++pos; // skip '['
    int depth = 0;
    size_t obj_start = std::string::npos;
    bool in_string = false;
    char prev = 0;
    for (; pos < s.size(); ++pos) {
        char c = s[pos];
        if (c == '"' && prev != '\\') in_string = !in_string;
        if (!in_string) {
            if (c == '{') {
                if (depth == 0) obj_start = pos;
                ++depth;
            }
            else if (c == '}') {
                --depth;
                if (depth == 0 && obj_start != std::string::npos) {
                    objs.push_back(s.substr(obj_start, pos - obj_start + 1));
                    obj_start = std::string::npos;
                }
            }
        }
        prev = c;
    }
    return objs;
}

static bool extract_field(const std::string& obj, const std::string& key, std::string& value, const std::vector<size_t>& quote_positions) {
    bool key_found = false;
    for (int i = 0; i < quote_positions.size(); i += 2) {
        std::string sub_str = obj.substr(quote_positions[i] + 1, quote_positions[i + 1] - quote_positions[i] - 1);
        if (key_found)
        {
            value = sub_str;
            return true;
        }

        if (sub_str == key)
            key_found = true;
    }
    return false;
}

std::unique_ptr<LexcemColorData> ConfigParser::parse(const std::string& fileName)
{
    auto result = std::make_unique<LexcemColorData>();

    auto fileTextPtr = ResourceProvider::GetFileText(fileName);
    if (!fileTextPtr) {
        std::cerr << "[ConfigParser] file not found: " << fileName << std::endl;
        return result;
    }

    std::string fileContent = fileTextPtr->data();
    if (fileContent.empty()) {
        std::cerr << "[ConfigParser] file empty: " << fileName << std::endl;
        return result;
    }

    auto objects = extract_top_level_objects(fileContent);
    if (objects.empty()) {
        std::cerr << "[ConfigParser] no objects found in JSON array" << std::endl;
        return result;
    }

    for (const auto& obj : objects) {
        std::vector<size_t> quote_positions;

        for (size_t i = 0; i < obj.size(); ++i) {
            if (obj[i] == '\'') {
                quote_positions.push_back(i);
            }
        }

        std::string typeStr;
        extract_field(obj, "Lecsem_type", typeStr, quote_positions);

        std::string filterStr;
        extract_field(obj, "Filter", filterStr, quote_positions);

        float r = 0.0f, g = 0.0f, b = 0.0f;
        std::string r_st;
        std::string g_st;
        std::string b_st;

        extract_field(obj, "R", r_st, quote_positions);
        extract_field(obj, "G", g_st, quote_positions);
        extract_field(obj, "B", b_st, quote_positions);

        r = std::stof(r_st);
        g = std::stof(g_st);
        b = std::stof(b_st);
        
        if (typeStr.empty() && filterStr.empty()) continue;

        auto entry = std::make_unique<LexcemDataType>(typeStr, filterStr, Color(r, g, b));
        result->add(std::move(entry));
    }

    return result;
}
