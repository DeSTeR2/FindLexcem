#pragma once
#include <string>

#include "Color.h"
#include "regex"

class LexcemDataType {
private: 
	std::string m_Type;
	std::string m_Filter;
	Color m_Color;
	std::regex m_Regex;

public:
	LexcemDataType(std::string type, std::string filter, Color color);

	std::string GetType();
	std::regex GetRegex();
	Color GetColor();
};