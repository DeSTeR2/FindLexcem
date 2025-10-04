#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "LexcemDataType.h"

class LexcemColorData {
private:
	std::vector<std::unique_ptr<LexcemDataType>> data;

public:
	void add(std::unique_ptr<LexcemDataType> dataType);
	std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>>get_data();
};