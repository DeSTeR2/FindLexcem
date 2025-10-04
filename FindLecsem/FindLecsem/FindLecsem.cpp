#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "ConfigParser.h"
#include "ResourceProvider.h"
#include "LexcemColorData.h"
#include "ConsoleColor.h"
#include "ConfigProvider.h"
#include "Token.h"

std::unordered_set<std::string> type_keywords = {
    "int","short","long","float","double","char","bool","void",
    "std::string","std::wstring","vector","list","deque","set",
    "unordered_set","map","unordered_map","stack","queue",
    "priority_queue","Point","MyClass","Node"
};

std::unordered_map<std::string, std::string> variable_types;

bool isIdentifierChar(char c) {
    return std::isalnum(c) || c == '_';
}

bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    size_t i = 0;
    size_t n = code.size();
    std::string current_type;

    while (i < n) {
        char c = code[i];

        if (isWhitespace(c)) {
            std::string ws;
            while (i < n && isWhitespace(code[i])) ws += code[i++];
            tokens.push_back({ ws, "Whitespace" });
        }
        else if (c == '/' && i + 1 < n && code[i + 1] == '/') {
            std::string comment = "//";
            i += 2;
            while (i < n && code[i] != '\n') comment += code[i++];
            tokens.push_back({ comment, "Comment" });
        }
        else if (c == '/' && i + 1 < n && code[i + 1] == '*') {
            std::string comment = "/*";
            i += 2;
            while (i + 1 < n && !(code[i] == '*' && code[i + 1] == '/')) comment += code[i++];
            if (i + 1 < n) { comment += "*/"; i += 2; }
            tokens.push_back({ comment, "Comment" });
        }
        else if (c == 'R' && i + 1 < n && code[i + 1] == '"') {
            std::string rawstr = "R\"";
            i += 2;
            std::string delim;
            while (i < n && code[i] != '(') delim += code[i++];
            rawstr += delim + "(";
            if (i < n) i++;
            std::string content;
            std::string end_delim = ")" + delim + "\"";
            while (i < n && code.substr(i, end_delim.size()) != end_delim) content += code[i++];
            rawstr += content;
            if (i + end_delim.size() - 1 < n) { rawstr += end_delim; i += end_delim.size(); }
            tokens.push_back({ rawstr, "RawString" });
        }
        else if (c == '"') {
            std::string str = "\"";
            i++;
            while (i < n) {
                str += code[i];
                if (code[i] == '\\' && i + 1 < n) { i++; str += code[i]; }
                else if (code[i] == '"') break;
                i++;
            }
            i++;
            tokens.push_back({ str, "String" });
        }
        else if (c == '\'') {
            std::string chr = "\'";
            i++;
            while (i < n) {
                chr += code[i];
                if (code[i] == '\\' && i + 1 < n) { i++; chr += code[i]; }
                else if (code[i] == '\'') break;
                i++;
            }
            i++;
            tokens.push_back({ chr, "Char" });
        }
        else if (c == '#') {
            std::string preproc;
            while (i < n && code[i] != '\n') preproc += code[i++];
            tokens.push_back({ preproc, "Preprocessor" });
        }
        else if (std::string("{}[](),;<>~!%^&*+-=/?:.").find(c) != std::string::npos) {
            tokens.push_back({ std::string(1, c), "Operator" });
            i++;
        }
        else if (isIdentifierChar(c)) {
            std::string id;
            while (i < n && isIdentifierChar(code[i])) id += code[i++];

            std::string type = "Identifier";

            if (type_keywords.count(id)) {
                type = "Type";
                current_type = id; 
            }
            else if (!current_type.empty()) {
                
                type = "Variable";
                variable_types[id] = current_type;
                current_type.clear();
            }
            else if (variable_types.count(id)) {
                type = "Variable";
            }

            tokens.push_back({ id, type });
        }
        else {
            tokens.push_back({ std::string(1, c), "Unknown" });
            i++;
        }
    }

    return tokens;
}

void detectFunctionCalls(std::vector<Token>& tokens) {
    for (size_t i = 0; i + 1 < tokens.size(); ++i) {
        
        if (tokens[i].type == "Identifier" && tokens[i + 1].text == "(") {
            tokens[i].type = "FunctionCall";
        }
        
        else if (tokens[i].type == "Variable" && tokens[i + 1].text == ".") {
            if (i + 2 < tokens.size() && tokens[i + 2].type == "Identifier" && i + 3 < tokens.size() && tokens[i + 3].text == "(") {
                tokens[i + 2].type = "MemberFunctionCall";
            }
        }
    }
}

void process_token(const Token& token, const std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>>& configs) {
    if (token.text.empty()) return;

    for (const auto& config : *configs) {
        if (std::regex_search(token.text, config->GetRegex())) {
            ConsoleColor::print(token.text, config->GetColor());
            return;
        }
    }

    if (token.type == "Type") ConsoleColor::print(token.text, Color(220, 50, 50));
    else if (token.type == "Variable") ConsoleColor::print(token.text, Color(255, 200, 0));
    else if (token.type == "FunctionCall") ConsoleColor::print(token.text, Color(0, 255, 0));
    else if (token.type == "MemberFunctionCall") ConsoleColor::print(token.text, Color(0, 180, 255));
    else ConsoleColor::print(token.text, Color(255, 255, 255));
}

int main() {
    auto configs = ConfigProvider::get_data();
    auto test_file = ResourceProvider::GetFileText("test_file.cpp");
    const std::string& code = test_file->data();

    auto tokens = tokenize(code);
    detectFunctionCalls(tokens);

    for (const auto& token : tokens) {
        process_token(token, configs);
    }

    return 0;
}
