#include "CountryContainer.h"

void CountryContainer::initializeData(FilePathHandler*& filePathHandler)
{
    const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::COUNTRY_TAGS_).at(0).string();
    for (const auto& path : filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::COUNTRY_TAGS_)) {

        std::ifstream file(path, std::ios::binary);
        if (!file) throw std::runtime_error("failed to open file");

        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        std::string buffer(size, '\0');
        file.read(buffer.data(), size);
        file.close();

        const char* ptr = buffer.data();
        const char* end = ptr + size;
        const char* keyStart = nullptr;

        while (ptr < end) {
            switch (*ptr) {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
            case '#': {
                while (*ptr != '\n') ++ptr;
                break;
            }
            default: {
                std::string tag(ptr, 3);
                std::string name;
                std::string commonKeyName;
                std::string historyKeyName(ptr, 3);
                historyKeyName.append(" - ");
                ptr += 3;

                if (*(ptr + 13) == '/') ptr += 14;
                else {
                    while (*ptr != '/' && *ptr != '\\') ++ptr;
                    std::cout << "Potential problem in" + path.string() + "deviating from standard template" << std::endl;
                }

                keyStart = ptr;
                while (*ptr != '.') ++ptr;
                commonKeyName.assign(keyStart, ptr);
                name.assign(keyStart, ptr);
                ptr += 4;

                commonKeyName.assign(keyStart, ptr);
                historyKeyName.append(keyStart, ptr);

                tagToName[tag] = std::move(name);
                tagToCommonKey[tag] = std::move(commonKeyName);
                tagToHistoryKey[tag] = std::move(historyKeyName);
            }
            }
            ++ptr;
        }
    }
}
