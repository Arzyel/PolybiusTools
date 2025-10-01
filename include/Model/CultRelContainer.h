#pragma once
#ifndef CULT_REL_CONTAINER_H
#define CULT_REL_CONTAINER_H
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>



struct sCulture {
    uint32_t packedRGB;
    char primaryTag[4] = {};
    std::string cultureGroupID;

    void setprimaryTag(char a, char b, char c) {
        primaryTag[0] = a;
        primaryTag[1] = b;
        primaryTag[2] = c;
    }
};

struct sCultureGroup {
    uint32_t packedRGB;
    std::string graphicalCulture;
    std::string secondGraphicalCulture;
};

struct sReligion {
    uint32_t packedRGB;
    std::string religionGroupID;
};

struct sReligionGroup {
    uint32_t packedRGB;

};

class CultRelContainer {
public:
    CultRelContainer();
    ~CultRelContainer() = default;
    void loadCultureData(const std::string& cultureFilePath);
    void loadReligionData(const std::string& religionFilePath);

protected:

private:
    std::unordered_map<std::string, sCulture> mCultures;
    std::unordered_map<std::string, sCultureGroup> mCultureGroups;
    std::unordered_map<std::string, sReligion> mReligions;
    std::unordered_map<std::string, sReligionGroup> mReligionGroups;

};

#endif // CULT_REL_CONTAINER_H
