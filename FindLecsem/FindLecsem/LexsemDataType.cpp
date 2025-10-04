#include "LexcemDataType.h"

LexcemDataType::LexcemDataType(std::string type, std::string filter, Color color)
{
	m_Type = type;
	m_Filter = filter;
	m_Color = color;

	m_Regex = std::regex(m_Filter);
}

std::string LexcemDataType::GetType(){
	return m_Filter;
}

std::regex LexcemDataType::GetRegex()
{
	return m_Regex;
}

Color LexcemDataType::GetColor()
{
	return m_Color;
}
