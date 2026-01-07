#include "CountryContainer.h"

void CountryContainer::initializeData(FilePathHandler*& filePathHandler)
{
    const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::COUNTRY_TAGS_).at(0).string();
    std::ifstream file(path);
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line)) {

        auto commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);

        line.erase(std::remove_if(line.begin(), line.end(),
            [](unsigned char c) { return std::isspace(c); }),
            line.end());

        if (line.empty())
            continue;

        auto eqPos = line.find('=');
        if (eqPos == std::string::npos)
            continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        auto lastSlash = value.find_last_of("/\\");
        auto lastDot = value.find_last_of('.');
        if (lastDot == std::string::npos) lastDot = value.size();
        value = value.substr(lastSlash + 1, lastDot - lastSlash - 1);

        tagToName[key] = value;
    }
    file.close();
}
