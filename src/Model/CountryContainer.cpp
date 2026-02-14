#include "CountryContainer.h"

void CountryContainer::initializeData(FilePathHandler*& filePathHandler)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Initiate Country Data\t----\t";

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

                mNames.emplace_back(name);
                tagToIndex[tag] = mNames.size() - 1;
                tagToName[tag] = std::move(name);

                mFolderCommonKeys.emplace_back(std::move(commonKeyName));
                mFolderHistoryKeys.emplace_back(std::move(historyKeyName));
            }
            }
            ++ptr;
        }
    }

    // hack why eu4 change its naming rule for that is beyond me
    // the hack doesnt even work somehow the key isnt the filePathHandler
    mFolderHistoryKeys[tagToIndex["REB"]] = "REB - Rebel Scum.txt";

    mCountriesData.resize(mNames.size());
    initHistoryData(filePathHandler);
    auto time_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - start);
    std::cout << "Elapsed Time : " << elapsed.count() << "ms" << std::endl;
}

void CountryContainer::initHistoryData(FilePathHandler*& filePathHandler)
{
    std::vector<std::string> absoluteImportPaths;
    absoluteImportPaths.resize(mFolderHistoryKeys.size());
    for (int i = 3; i < mFolderHistoryKeys.size(); ++i) {
        absoluteImportPaths[i] = (filePathHandler->getAbsolutePathFromFileKey(mFolderHistoryKeys[i]));
    }
    for (int i = 3; i < absoluteImportPaths.size(); ++i) {
        //mCountriesData[i].initHistory(absoluteImportPaths[i]);
    }


}

void CountryContainer::initCommonData(FilePathHandler*& filePathHandler)
{
}

Eu4::Country::~Country()
{
    delete mHistoryFileData;
    delete mCommonFileData;
}

void Eu4::Country::initHistory(const std::string& path, const std::string& exportPath)
{
    //mHistoryFileData = new DM::FileData<Eu4::Country>(path, exportPath, )
}
