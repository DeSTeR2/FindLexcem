#include <iostream>
#include <fstream>
#include <regex>
#include <memory>
#include <vector>
#include <string>
#include "ConfigParser.h"
#include "ResourceProvider.h"
#include "LexcemColorData.h"
#include "ConsoleColor.h"

void process_token(const std::string& token, const std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>>& configs) {
    for (const auto& config : *configs) {
        if (std::regex_match(token, config->GetRegex())) {
            ConsoleColor::print(token, config->GetColor());
            return;
        }
    }
}

int main() {
    auto configs = ConfigParser::parse("config.json")->get_data();
    for (int i = 0; i < configs->size(); i++) {
        std::cout << configs->at(i).get()->GetType() << '\n';
    }

    auto test_file = ResourceProvider::GetFileText("test_file.cpp");
    const std::string& code = test_file->data();

    const std::regex token_regex(
        R"((//.*?$|/\*[\s\S]*?\*/|"(\\.|[^"])*"|'(\\.|[^'])'|\b\w+\b|==|!=|<=|>=|->\*|->|::|\.\*|\.\.|\.|\+\+|--|\+=|-=|\*=|/=|%=|&&|\|\||[{}()\[\],;<>~!%^&*+\-=/?:]|\s+))",
        std::regex::icase
    );

    auto begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        std::string token = it->str();
        process_token(token, configs);
    }

    return 0;
}
