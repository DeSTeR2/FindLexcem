#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include "Constants.h"

static class ResourceProvider {
private:
	static std::unique_ptr<std::ifstream> LoadFile(std::string path);
public: 
	static std::unique_ptr<std::string> GetFileText(std::string path);
};