#include "LexcemColorData.h"

void LexcemColorData::add(std::unique_ptr< LexcemDataType> dataType)
{
	data.emplace_back(std::move(dataType));
}

std::shared_ptr<std::vector<std::unique_ptr<LexcemDataType>>> LexcemColorData::get_data()
{
    return std::make_shared<std::vector<std::unique_ptr<LexcemDataType>>>(std::move(data));
}
