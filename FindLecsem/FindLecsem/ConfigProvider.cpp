#include "ConfigProvider.h"
#include "LexcemDataType.h"
#include "Color.h"
#include <memory>
#include <vector>
#include <regex>

std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>>& ConfigProvider::get_data()
{
    static std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>> data =
        std::make_shared<std::vector<std::unique_ptr<LexcemDataType>>>();

    if (!data->empty()) return data; // already initialized

    // Preprocessor_directives
    data->push_back(std::make_unique<LexcemDataType>(
        "Preprocessor_directives",
        std::regex(R"(#\s*(include|define|undef|if|ifdef|ifndef|elif|else|endif|pragma|error|line)\b)"),
        Color(180, 90, 220)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Strings",
        std::regex(R"("(\\.|[^"])*")"),
        Color(180, 120, 20)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Raw_strings",
        std::regex(R"(R"([^\s()\\]{0,16})\((.*?)\)\1")", std::regex::ECMAScript),
        Color(180, 100, 50)
    ));


    data->push_back(std::make_unique<LexcemDataType>(
        "Character_literals",
        std::regex(R"('(?:\\.|[^\\'])')"),
        Color(200, 100, 50)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Keywords",
        std::regex(R"(\b(alignas|alignof|and|and_eq|asm|auto|bool|break|case|catch|char|class|const|constexpr|continue|decltype|default|delete|do|double|else|enum|explicit|export|extern|false|float|for|friend|goto|if|inline|int|long|mutable|namespace|new|noexcept|nullptr|operator|private|protected|public|register|reinterpret_cast|return|short|signed|sizeof|static|struct|switch|template|this|thread_local|throw|true|try|typedef|typeid|typename|union|unsigned|using|virtual|void|volatile|while)\b)"),
        Color(220, 50, 50)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Operators",
        std::regex(R"(->\*|->|::|\.\\*|\.{2}|==|!=|<=|>=|<<=|>>=|<<|>>|\+\+|--|\+=|-=|\*=|/=|%=|&=|\|=|\^=|&&|\|\||\?|:|\+|-|\*|/|%|<|>|=|&|\||\^|~|!)"),
        Color(80, 80, 80)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Punctuators_and_delimiters",
        std::regex(R"([{}()\[\],;.])"),
        Color(60, 60, 60)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Function_calls",
        std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\s*(?=\())"),
        Color(0, 180, 180)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Std_identifiers",
        std::regex(R"(\bstd|cout|cin\b)"),
        Color(100, 150, 255)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Vector_type",
        std::regex(R"(\bvector\b)"),
        Color(150, 100, 255)
    ));

    data->push_back(std::make_unique<LexcemDataType>(
        "Numeric_literals",
        std::regex(R"(\b(0b[01]+|0x[0-9A-Fa-f]+|\d+\.?\d*([eE][+-]?\d+)?|\.\d+)([uUlLfF]*)\b)"),
        Color(28, 120, 255)
    ));

    //data->push_back(std::make_unique<LexcemDataType>(
    //    "Default",
    //    std::regex(R"(.*)"),
    //    Color(255, 255, 255)
    //));

    return data;
}
