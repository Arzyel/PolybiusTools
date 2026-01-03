// ============================================================================
// CultRelContainer.h
// ============================================================================
#pragma once
#ifndef CULT_REL_CONTAINER_H
#define CULT_REL_CONTAINER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <string_view>
#include "FileManager.h"
#include "filenfolder_CONST.h"
#include "FilePathHandler.h"

#include "Eu4MainParser.h"


namespace fs = std::filesystem;
namespace Eu4 {
    class Culture {
    public:
        uint16_t mNameID;
        uint16_t mGroupID;
    };

    class CultureGroup {
        // for names either a data token per name which could be costlier than using one data token and having the value being one string
        // it the class it would store the vector of string which would rebuild the complete string afterward
        // for now dont care and simply skip it only need nameID and mCulturesID
    public:
        uint16_t mNameID;
        uint16_t mGFXID;
        uint16_t mMaleNamesID;
        uint16_t mFemaleNamesID;
        uint16_t mDynastyNamesID;
        std::vector<uint16_t> mCulturesID;
    };

    class Religion {
    public:
        uint16_t mNameID;
        uint16_t mGroupID;
    };

    class ReligionGroup {
    public:
        uint16_t mNameID;
    };
}


class CultRelContainer {
public:
    CultRelContainer();
    ~CultRelContainer();
    
    void initData(FilePathHandler*& filePathHandler);
    const std::vector<std::string_view> getAllCultures() const;
    const std::vector<std::string_view> getAllReligions() const;

private:
    //might need a map to string to value but unsure at the moment
    DM::FileData<CultRelContainer>* mCultureData = nullptr;
    DM::FileData<CultRelContainer>* mReligionData = nullptr;
    std::vector<Eu4::Culture> mCulturestest;
    std::vector<Eu4::CultureGroup> mCultureGroupstest;
    std::vector<Eu4::Religion> mReligionstest;
    std::vector<Eu4::ReligionGroup> mReligionGroupstest;


    static void parserSkipBracket(const char*& ptr, const char* end);
    
    void initDataCulture(FilePathHandler*& filePathHandler);
    static void initHelperCulture(DM::FileData<CultRelContainer>& fileData, CultRelContainer& cultRelContainer);
    static void resetCulture(CultRelContainer& cultRelContainer);

    void initDataReligion(FilePathHandler*& filePathHandler);
    static void initHelperReligion(DM::FileData<CultRelContainer>& fileData, CultRelContainer& cultRelContainer);
    static void resetReligion(CultRelContainer& cultRelContainer);

};


#endif // CULT_REL_CONTAINER_H