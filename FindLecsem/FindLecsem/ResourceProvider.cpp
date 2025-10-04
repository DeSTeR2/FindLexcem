#include "ResourceProvider.h"

std::unique_ptr<std::ifstream> ResourceProvider::LoadFile(std::string path)
{
    std::string full_path = RESOURCE_PATH + path;
    auto file = std::make_unique<std::ifstream>(full_path, std::ios::binary);
    if (!file->is_open()) {
        return nullptr;
    }
    return file;
}

std::unique_ptr<std::string> ResourceProvider::GetFileText(std::string path)
{
    std::unique_ptr<std::ifstream> file = LoadFile(path);
    std::string file_content = "";
    if (file) {
        std::string line;
        while (std::getline(*file, line)) {
            file_content += line + '\n';
        }
    }

    file.get()->close();
    file.release();
    return std::make_unique<std::string>(file_content);
}
