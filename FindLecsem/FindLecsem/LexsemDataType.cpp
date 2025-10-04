#include "LexcemDataType.h"

LexcemDataType::LexcemDataType(std::string type, std::regex regex, Color color)
{
	m_Type = type;
	m_Regex = regex;
	m_Color = color;
}

std::string LexcemDataType::GetType(){
	return m_Type;
}

std::regex LexcemDataType::GetRegex()
{
	return m_Regex;
}

Color LexcemDataType::GetColor()
{
	return m_Color;
}
