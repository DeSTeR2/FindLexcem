#pragma once

#include <iostream>

#include "LexcemColorData.h"

class ConfigParser {
public: 
	static std::unique_ptr<LexcemColorData> parse(const std::string& fileName);
};	