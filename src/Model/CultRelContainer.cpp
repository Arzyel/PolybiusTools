// ============================================================================
// CultRelContainer.cpp
// ============================================================================
#include "CultRelContainer.h"
#include "TGContainer.h"


CultRelContainer::CultRelContainer() {
}

CultRelContainer::~CultRelContainer()
{
    for (auto& cultureData : mCulturesPerFile) {
        delete cultureData.mCultureData;
    }
    mCulturesPerFile.clear();
    for (auto& religionData : mReligionsPerFile) {
        delete religionData.mReligionData;
    }
    mReligionsPerFile.clear();
}

void CultRelContainer::initData(FilePathHandler*& filePathHandler)
{
    initDataCulture(filePathHandler);
    initDataReligion(filePathHandler);
}

const std::vector<std::string_view> CultRelContainer::getAllCultures() const
{
    std::vector<std::string_view> cultures;
    uint8_t size = 0;
    for (const auto& cultureDataPerFile : mCulturesPerFile) {
        size += cultureDataPerFile.mCultures.size();
    }
    cultures.reserve(size);
    for (const auto& cultureData : mCulturesPerFile) {
        for (const auto& culture : cultureData.mCultures) {
            auto& token = cultureData.mCultureData->mDataTokens[culture.mNameID];
            cultures.emplace_back(token.mPtrStart, token.mLength);
        }
    }
    return cultures;
}

const std::vector<std::string_view> CultRelContainer::getAllReligions() const
{
    std::vector<std::string_view> religions;

    uint8_t size = 0;
    for (const auto& religionDataPerFile : mReligionsPerFile) {
        size += religionDataPerFile.mReligions.size();
    }
    religions.reserve(size);
    for (const auto& religionDataPerFile : mReligionsPerFile) {
        for (const auto& religion : religionDataPerFile.mReligions) {
            auto& token = religionDataPerFile.mReligionData->mDataTokens[religion.mNameID];
            religions.emplace_back(token.mPtrStart, token.mLength);
        }
    }
    return religions;
}

void CultRelContainer::initDataCulture(FilePathHandler*& filePathHandler)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Initiate Culture Data from file\t----\t";

    for (auto& path : filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::CULTURES_)) {
        auto& culture = mCulturesPerFile.emplace_back();
        std::string pathStr = path.string();
        culture.mCultureData = new DM::FileData<Eu4::CultureDataPerFile>(
            pathStr,
            filePathHandler->getExportFromFullPath(pathStr)
        );
        culture.mCultureData->initData(
            Eu4::CultureDataPerFile::initHelperCulture,
            &culture,
            Eu4::CultureDataPerFile::resetCulture
        );
    }


    auto time_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - start);
    std::cout << "Elapsed Time : " << elapsed.count() << "ms" << std::endl;
}


void CultRelContainer::initDataReligion(FilePathHandler*& filePathHandler)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Initiate Religion Data from file\t----\t";

    for (auto& path : filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::RELIGIONS_)) {
        auto& religion = mReligionsPerFile.emplace_back();
        std::string pathStr = path.string();
        religion.mReligionData = new DM::FileData<Eu4::ReligionDataPerFile>(
            pathStr,
            filePathHandler->getExportFromFullPath(pathStr)
        );
        religion.mReligionData->initData(
            Eu4::ReligionDataPerFile::initHelperReligion,
            &religion,
            Eu4::ReligionDataPerFile::resetReligion
        );
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - start);
    std::cout << "Elapsed Time : " << elapsed.count() << "ms" << std::endl;
}





void Eu4::CultureDataPerFile::initHelperCulture(DM::FileData<CultureDataPerFile>& fileData, CultureDataPerFile& cultDataPerFile)
{
    const char* ptr = fileData.mBuffer.data();
    const char* end = ptr + fileData.mBuffer.size();

    const char* keyStart = nullptr;
    while (ptr < end) {
        switch (*ptr) {
        case '#': {
            const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
            ptr = commentEnd ? commentEnd : end;

            break;
        }
        case '{': {
            ++ptr;
            while (*ptr != '}') {
                while (ptr < end && (*ptr < 'a' || *ptr > 'z') && *ptr != '}' && *ptr != '#') ++ptr;
                char c = *ptr;
                if (c == '}') continue;
                else if (c == '#') {
                    while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                    continue;
                }
                //graphical_culture
                if (c == 'g' && *(ptr + 16) == 'e') {
                    ptr += 17;
                    while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                }
                //second_graphical_culture
                else if (c == 's' && *(ptr + 23) == 'e') {
                    ptr += 24;
                    while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                }
                //male_names
                else if (c == 'm' && *(ptr + 9) == 's') {
                    ptr += 10;
                    while (ptr < end && *ptr != '{') ++ptr;
                    Eu4::parserSkipBracket(ptr, end);
                }
                //female_names
                else if (c == 'f' && *(ptr + 11) == 's') {
                    ptr += 12;
                    while (ptr < end && *ptr != '{') ++ptr;
                    Eu4::parserSkipBracket(ptr, end);
                }
                //dynasty_names
                else if (c == 'd' && *(ptr + 12) == 's') {
                    ptr += 13;
                    while (ptr < end && *ptr != '{') ++ptr;
                    Eu4::parserSkipBracket(ptr, end);
                }
                //province
                else if (c == 'p' && *(ptr + 7) == 'e') {
                    ptr += 8;
                    while (ptr < end && *ptr != '{') ++ptr;
                    Eu4::parserSkipBracket(ptr, end);
                }
                else {
                    keyStart = ptr;
                    ptr += strcspn(ptr, "\n\r \t#=");
                    cultDataPerFile.mCultures.emplace_back(Eu4::Culture());
                    Eu4::Culture& cult = cultDataPerFile.mCultures.back();
                    fileData.mDataTokens.emplace_back(DM::DataToken());
                    fileData.mDataTokens.back().mPtrStart = keyStart;
                    fileData.mDataTokens.back().mLength = ptr - keyStart;
                    cult.mNameID = fileData.mDataTokens.size() - 1;
                    cult.mGroupID = cultDataPerFile.mCultureGroups.back().mNameID;
                    keyStart = nullptr;
                    ptr += strcspn(ptr, "{");
                    Eu4::parserSkipBracket(ptr, end);
                }
                ++ptr;
            }
            break;
        }
        case '\n':
        case '\r':
        case '\t':
        case ' ':
        case '=': {
            break;
        }
        default: {
            keyStart = ptr;
            while (*ptr != '\n' && *ptr != '\r' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
                ++ptr;
            }
            cultDataPerFile.mCultureGroups.emplace_back(Eu4::CultureGroup());
            Eu4::CultureGroup& cultGroup = cultDataPerFile.mCultureGroups.back();
            fileData.mDataTokens.emplace_back(DM::DataToken());
            fileData.mDataTokens.back().mPtrStart = keyStart;
            fileData.mDataTokens.back().mLength = ptr - keyStart;
            cultGroup.mNameID = fileData.mDataTokens.size() - 1;
            keyStart = nullptr;
        }
        }
        ++ptr;
    }
}

void Eu4::CultureDataPerFile::resetCulture(CultureDataPerFile& cultRelContainer)
{
}

void Eu4::ReligionDataPerFile::initHelperReligion(DM::FileData<ReligionDataPerFile>& fileData, ReligionDataPerFile& religionDataPerFile)
{
    const char* ptr = fileData.mBuffer.data();
    const char* end = ptr + fileData.mBuffer.size();

    const char* keyStart = nullptr;
    while (ptr < end) {
        switch (*ptr) {
        case '#': {
            const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
            ptr = commentEnd ? commentEnd : end;

            break;
        }
        case '{': {
            ++ptr;
            while (*ptr != '}' && ptr < end) {
                while (ptr < end && (*ptr < 'a' || *ptr > 'z') && *ptr != '#' && *ptr != '}') ++ptr;
                if (*ptr == '#') {
                    while (*ptr != '\n' && *ptr != '\r') ++ptr;
                    continue;
                }
                else if (*ptr == '}') {
                    continue;
                }
                char c = *ptr;
                bool matched = true;

                switch (c) {
                case 'd':
                    // defender_of_faith
                    if (*(ptr + 16) == 'h') {
                        ptr += 17;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    matched = false;
                    break;
                case 'c':
                    //can_form_personal_unions
                    if (*(ptr + 23) == 's') {
                        ptr += 24;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    //center_of_religion
                    else if (*(ptr + 17) == 'n') {
                        ptr += 18;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    //crusade_name
                    else if (*(ptr + 11) == 'e') {
                        ptr += 12;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    matched = false;
                    break;
                case 'f':
                    //flags_with_emblem_percentage
                    if (*(ptr + 27) == 'e') {
                        ptr += 28;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    //flag_emblem_index_range
                    else if (*(ptr + 22) == 'e') {
                        ptr += 23;
                        while (*ptr != '{' && ptr < end) ++ptr;
                        Eu4::parserSkipBracket(ptr, end);
                        break;
                    }
                    matched = false;
                    break;
                case 'h':
                    //harmonized_modifier
                    if (*(ptr + 10) == '_' && *(ptr + 18) == 'r') {
                        ptr += 19;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    matched = false;
                    break;
                case 'a':
                    //ai_will_propagate_through_trade
                    if (*(ptr + 30) == 'e') {
                        ptr += 31;
                        while (ptr < end && *ptr != '\n' && *ptr != '\r') ++ptr;
                        break;
                    }
                    matched = false;
                    break;
                case 'r':
                    //religious_schools
                    if (*(ptr + 16) == 's') {
                        ptr += 17;
                        while (*ptr != '{' && ptr < end) ++ptr;
                        Eu4::parserSkipBracket(ptr, end);
                        break;
                    }
                    matched = false;
                    break;
                default:
                    matched = false;
                    break;
                }

                if (!matched) {
                    keyStart = ptr;
                    ptr += strcspn(ptr, "\n\r \t#=");
                    religionDataPerFile.mReligions.emplace_back(Eu4::Religion());
                    Eu4::Religion& rel = religionDataPerFile.mReligions.back();
                    fileData.mDataTokens.emplace_back(DM::DataToken());
                    fileData.mDataTokens.back().mPtrStart = keyStart;
                    fileData.mDataTokens.back().mLength = ptr - keyStart;
                    rel.mNameID = fileData.mDataTokens.size() - 1;
                    rel.mGroupID = religionDataPerFile.mReligionGroups.back().mNameID;
                    keyStart = nullptr;
                    ptr += strcspn(ptr, "{");
                    Eu4::parserSkipBracket(ptr, end);
                }

                ++ptr;

            }
            break;
        }
        case '\n':
        case '\r':
        case '\t':
        case ' ':
        case '=': {
            break;
        }
        default: {
            keyStart = ptr;
            while (*ptr != '\n' && *ptr != '\r' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
                ++ptr;
            }
            religionDataPerFile.mReligionGroups.emplace_back(Eu4::ReligionGroup());
            Eu4::ReligionGroup& relGroup = religionDataPerFile.mReligionGroups.back();
            fileData.mDataTokens.emplace_back(DM::DataToken());
            fileData.mDataTokens.back().mPtrStart = keyStart;
            fileData.mDataTokens.back().mLength = ptr - keyStart;
            relGroup.mNameID = fileData.mDataTokens.size() - 1;
            keyStart = nullptr;
        }
        }
        ++ptr;
    }
}

void Eu4::ReligionDataPerFile::resetReligion(ReligionDataPerFile& cultRelContainer)
{
}
