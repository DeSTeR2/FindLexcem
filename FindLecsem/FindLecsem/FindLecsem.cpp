#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "ConfigParser.h"
#include "ResourceProvider.h"
#include "LexcemColorData.h"
#include "ConsoleColor.h"
#include "ConfigProvider.h"

// Helper: check if char is part of identifier
bool isIdentifierChar(char c) {
    return std::isalnum(c) || c == '_';
}

// Helper: check if char is whitespace
bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Tokenizer
std::vector<std::string> tokenize(const std::string& code) {
    std::vector<std::string> tokens;
    size_t i = 0;
    size_t n = code.size();

    while (i < n) {
        char c = code[i];

        // 1. Whitespace (preserve it)
        if (isWhitespace(c)) {
            std::string ws;
            while (i < n && isWhitespace(code[i])) {
                ws += code[i++];
            }
            tokens.push_back(ws);
        }
        // 2. Single-line comment
        else if (c == '/' && i + 1 < n && code[i + 1] == '/') {
            std::string comment = "//";
            i += 2;
            while (i < n && code[i] != '\n') comment += code[i++];
            tokens.push_back(comment);
        }
        // 3. Multi-line comment
        else if (c == '/' && i + 1 < n && code[i + 1] == '*') {
            std::string comment = "/*";
            i += 2;
            while (i + 1 < n && !(code[i] == '*' && code[i + 1] == '/')) {
                comment += code[i++];
            }
            if (i + 1 < n) {
                comment += "*/";
                i += 2;
            }
            tokens.push_back(comment);
        }
        // 4. Raw string literal R"( ... )"
        else if (c == 'R' && i + 1 < n && code[i + 1] == '"') {
            std::string rawstr = "R\"";
            i += 2;
            // capture delimiter
            std::string delim;
            while (i < n && code[i] != '(') delim += code[i++];
            rawstr += delim + "(";
            if (i < n) i++; // skip '('
            std::string content;
            std::string end_delim = ")" + delim + "\"";
            while (i < n && code.substr(i, end_delim.size()) != end_delim) {
                content += code[i++];
            }
            rawstr += content;
            if (i + end_delim.size() - 1 < n) {
                rawstr += end_delim;
                i += end_delim.size();
            }
            tokens.push_back(rawstr);
        }
        // 5. String literal
        else if (c == '"') {
            std::string str = "\"";
            i++;
            while (i < n) {
                str += code[i];
                if (code[i] == '\\' && i + 1 < n) {
                    i++;
                    str += code[i]; // escape sequence
                }
                else if (code[i] == '"') break;
                i++;
            }
            i++;
            tokens.push_back(str);
        }
        // 6. Character literal
        else if (c == '\'') {
            std::string chr = "\'";
            i++;
            while (i < n) {
                chr += code[i];
                if (code[i] == '\\' && i + 1 < n) {
                    i++;
                    chr += code[i];
                }
                else if (code[i] == '\'') break;
                i++;
            }
            i++;
            tokens.push_back(chr);
        }
        // 7. Preprocessor directive
        else if (c == '#') {
            std::string preproc;
            while (i < n && code[i] != '\n') preproc += code[i++];
            tokens.push_back(preproc);
        }
        // 8. Multi-character operators
        else if ((i + 1 < n) &&
            ((code[i] == ':' && code[i + 1] == ':') ||
                (code[i] == '-' && code[i + 1] == '>') ||
                (code[i] == '+' && code[i + 1] == '+') ||
                (code[i] == '-' && code[i + 1] == '-') ||
                (code[i] == '=' && code[i + 1] == '=') ||
                (code[i] == '!' && code[i + 1] == '=') ||
                (code[i] == '<' && code[i + 1] == '=') ||
                (code[i] == '>' && code[i + 1] == '=')
                )) {
            tokens.push_back(code.substr(i, 2));
            i += 2;
        }
        // 9. Single-character operator or punctuator
        else if (std::string("{}[](),;<>~!%^&*+-=/?:.").find(c) != std::string::npos) {
            tokens.push_back(std::string(1, c));
            i++;
        }
        // 10. Identifier or keyword
        else if (isIdentifierChar(c)) {
            std::string id;
            while (i < n && isIdentifierChar(code[i])) id += code[i++];
            tokens.push_back(id);
        }
        else {
            // fallback: single character
            tokens.push_back(std::string(1, c));
            i++;
        }
    }

    return tokens;
}

void process_token(const std::string& token, const std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>>& configs) {
    
    if (token.empty()) 
        return;

    for (const auto& config : *configs) {
        if (std::regex_search(token, config->GetRegex())) {
            ConsoleColor::print(token, config->GetColor());
            return;
        }
    }
}


int main() {
    auto configs = ConfigProvider::get_data();
    auto test_file = ResourceProvider::GetFileText("test_file.cpp");
    const std::string& code = test_file->data();
    auto tokens = tokenize(code);

    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        process_token(*it, configs);
    }

    for (const auto& token : tokens) {
        std::cout << "[" << token << "]\n";
    }

    return 0;
}
