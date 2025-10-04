#pragma once
#include <iostream>
#include <vector>
#include "LexcemDataType.h"

class ConfigProvider {
public: 
	static std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>>& get_data();
};