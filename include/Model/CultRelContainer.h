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
}
struct sCulture {
    uint32_t packedRGB;
    char primaryTag[4] = {};
    std::string cultureGroupID;
    std::vector<std::string> male;
    std::vector<std::string> female;
    std::vector<std::string> dynasty;
    
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
    std::vector<std::string> male;
    std::vector<std::string> female;
    std::vector<std::string> dynasty;
};

struct sReligion {
    std::string religionGroupID;
    uint32_t packedRGB;
    uint8_t icon;
};

struct sReligionGroup {
    uint32_t packedRGB;
    std::string defender_of_faith;
    std::string can_form_personal_unions;
    std::string ai_will_propagate_through_trade;
    uint16_t center_of_religion;
    uint8_t flags_with_emblem_percentage;
    std::vector<uint8_t> flag_emblem_index_range;
    std::string harmonized_modifier;
    std::string crusade_name;

};

class CultRelContainer {
public:
    CultRelContainer();
    ~CultRelContainer();
    
    void loadCultureData(const Node& node, const Eu4MainParser& parser);
    void loadCultureData(const std::vector<fs::path>& filePaths);
    void loadReligionData(const std::string& religionFilePath);
    void initData(FilePathHandler*& filePathHandler);
    
    //std::vector<Eu4::Continent> mContinents;
    //std::unordered_map<std::string, uint16_t> mContinentNameToIndex;
    //uint16_t mContinentDataID;
    //DM::FileData<Eu4::GeoPolData>* mContinentsData = nullptr;



    std::unordered_map<std::string, sCulture> mCultures;
    std::unordered_map<std::string, sCultureGroup> mCultureGroups;
    std::unordered_map<std::string, sReligion> mReligions;
    std::unordered_map<std::string, sReligionGroup> mReligionGroups;
private:
    DM::FileData<CultRelContainer>* mCultureData = nullptr;
    DM::FileData<CultRelContainer>* mReligionData = nullptr;
    std::vector<Eu4::Culture> mCulturestest;
    std::vector<Eu4::CultureGroup> mCultureGroupstest;

    void initDataCulture(FilePathHandler*& filePathHandler);
    static void initHelperCulture(DM::FileData<CultRelContainer>& fileData, CultRelContainer& cultRelContainer);
    static void resetCulture(CultRelContainer& cultRelContainer);
    static void parserSkipBracket(const char*& ptr, const char* end);

    void loadReligionDataHelper(const std::string& religionName, const Node& node, const Eu4MainParser& parser);
    
    // Helper functions
    static uint32_t parseRGBToPacked(const std::string& rgbStr);
    static void extractAllValues(const Node& node, std::vector<std::string>& result);
    
    // Property handler maps
    using CultureHandler = std::function<void(sCulture&, const Node&, const Eu4MainParser&)>;
    using CultureGroupHandler = std::function<void(sCultureGroup&, const Node&, const Eu4MainParser&)>;
    using ReligionHandler = std::function<void(sReligion&, const Node&, const Eu4MainParser&)>;
    using ReligionGroupHandler = std::function<void(sReligionGroup&, const Node&, const Eu4MainParser&)>;
    
    static const std::unordered_map<std::string, CultureHandler> cultureHandlers;
    static const std::unordered_map<std::string, CultureGroupHandler> cultureGroupHandlers;
    static const std::unordered_map<std::string, ReligionHandler> religionHandlers;
    static const std::unordered_map<std::string, ReligionGroupHandler> religionGroupHandlers;
    
    // Generic template functions
    template<typename T>
    static void parsePropertiesRecursive(
        T& obj, 
        const Node& node, 
        const Eu4MainParser& parser,
        const std::unordered_map<std::string, std::function<void(T&, const Node&, const Eu4MainParser&)>>& propertyHandlers
    );
    
    template<typename GroupType, typename ItemType>
    static void processGroupRecursive(
        const std::string& groupName,
        const Node& groupNode,
        const Eu4MainParser& parser,
        std::unordered_map<std::string, ItemType>& itemsMap,
        std::unordered_map<std::string, GroupType>& groupsMap,
        const std::unordered_map<std::string, std::function<void(ItemType&, const Node&, const Eu4MainParser&)>>& itemHandlers,
        const std::unordered_map<std::string, std::function<void(GroupType&, const Node&, const Eu4MainParser&)>>& groupHandlers,
        std::function<void(ItemType&, const std::string&)> setGroupID
    );
};

// Template implementations must be in header
template<typename T>
void CultRelContainer::parsePropertiesRecursive(
    T& obj, 
    const Node& node, 
    const Eu4MainParser& parser,
    const std::unordered_map<std::string, std::function<void(T&, const Node&, const Eu4MainParser&)>>& propertyHandlers) {
    
    for (const auto& [propID, propNode] : node.children) {
        std::string propKey = parser.intToStringID[propID];
        
        auto it = propertyHandlers.find(propKey);
        if (it != propertyHandlers.end()) {
            it->second(obj, propNode, parser);
        }
    }
}

template<typename GroupType, typename ItemType>
void CultRelContainer::processGroupRecursive(
    const std::string& groupName,
    const Node& groupNode,
    const Eu4MainParser& parser,
    std::unordered_map<std::string, ItemType>& itemsMap,
    std::unordered_map<std::string, GroupType>& groupsMap,
    const std::unordered_map<std::string, std::function<void(ItemType&, const Node&, const Eu4MainParser&)>>& itemHandlers,
    const std::unordered_map<std::string, std::function<void(GroupType&, const Node&, const Eu4MainParser&)>>& groupHandlers,
    std::function<void(ItemType&, const std::string&)> setGroupID) {
    
    GroupType group{};
    
    // First pass: parse group-level properties (including names that might be defined at group level)
    parsePropertiesRecursive(group, groupNode, parser, groupHandlers);
    
    // Second pass: process all children (individual items like cultures)
    for (const auto& [childID, childNode] : groupNode.children) {
        std::string childKey = parser.intToStringID[childID];
        
        if (!childNode.children.empty()) {
            // This is an item definition (culture or religion)
            ItemType item{};
            setGroupID(item, groupName);
            
            parsePropertiesRecursive(item, childNode, parser, itemHandlers);
            itemsMap[childKey] = item;
        }
    }
    
    groupsMap[groupName] = group;
}

#endif // CULT_REL_CONTAINER_H