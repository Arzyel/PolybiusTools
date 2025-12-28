// ============================================================================
// CultRelContainer.cpp
// ============================================================================
#include "CultRelContainer.h"
#include <sstream>
#include <algorithm>

CultRelContainer::CultRelContainer() {
}

CultRelContainer::~CultRelContainer()
{
    delete mCultureData;
    delete mReligionData;
}



uint32_t CultRelContainer::parseRGBToPacked(const std::string& rgbStr) {
    if (rgbStr.empty()) return 0;

    std::string cleaned = rgbStr;
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '{'), cleaned.end());
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '}'), cleaned.end());

    std::istringstream iss(cleaned);
    int r = 0, g = 0, b = 0;
    iss >> r >> g >> b;

    return ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

void CultRelContainer::extractAllValues(const Node& node, std::vector<std::string>& result) {
    if (!node.value.empty()) {
        result.push_back(node.value);
    }
    for (const auto& [childID, childNode] : node.children) {
        extractAllValues(childNode, result);
    }
}

// Initialize static handler maps
const std::unordered_map<std::string, CultRelContainer::CultureHandler>
CultRelContainer::cultureHandlers = {
    {"color", [](sCulture& c, const Node& n, const Eu4MainParser& p) {
        c.packedRGB = parseRGBToPacked(n.value);
    }},
    {"primary", [](sCulture& c, const Node& n, const Eu4MainParser& p) {
        if (n.value.length() >= 3) {
            c.setprimaryTag(n.value[0], n.value[1], n.value[2]);
        }
    }},
    {"male_names", [](sCulture& c, const Node& n, const Eu4MainParser& p) {
        extractAllValues(n, c.male);
    }},
    {"female_names", [](sCulture& c, const Node& n, const Eu4MainParser& p) {
        extractAllValues(n, c.female);
    }},
    {"dynasty_names", [](sCulture& c, const Node& n, const Eu4MainParser& p) {
        extractAllValues(n, c.dynasty);
    }}
};

const std::unordered_map<std::string, CultRelContainer::CultureGroupHandler>
CultRelContainer::cultureGroupHandlers = {
    {"color", [](sCultureGroup& cg, const Node& n, const Eu4MainParser& p) {
        cg.packedRGB = parseRGBToPacked(n.value);
    }},
    {"graphical_culture", [](sCultureGroup& cg, const Node& n, const Eu4MainParser& p) {
        cg.graphicalCulture = n.value;
    }},
    {"second_graphical_culture", [](sCultureGroup& cg, const Node& n, const Eu4MainParser& p) {
        cg.secondGraphicalCulture = n.value;
    }},
    {"male_names", [](sCultureGroup& cg, const Node& n, const Eu4MainParser& p) {
        extractAllValues(n, cg.male);
    }},
    {"female_names", [](sCultureGroup& cg, const Node& n, const Eu4MainParser& p) {
        extractAllValues(n, cg.female);
    }},
    {"dynasty_names", [](sCultureGroup& cg, const Node& n, const Eu4MainParser& p) {
        extractAllValues(n, cg.dynasty);
    }}
};

const std::unordered_map<std::string, CultRelContainer::ReligionHandler>
CultRelContainer::religionHandlers = {
    {"color", [](sReligion& r, const Node& n, const Eu4MainParser& p) {
        r.packedRGB = parseRGBToPacked(n.value);
    }}
};

const std::unordered_map<std::string, CultRelContainer::ReligionGroupHandler>
CultRelContainer::religionGroupHandlers = {
    {"color", [](sReligionGroup& rg, const Node& n, const Eu4MainParser& p) {
        rg.packedRGB = parseRGBToPacked(n.value);
    }}
};

void CultRelContainer::loadCultureData(const Node& rootNode, const Eu4MainParser& parser) {
    for (const auto& [groupID, groupNode] : rootNode.children) {
        std::string groupName = parser.intToStringID[groupID];

        processGroupRecursive<sCultureGroup, sCulture>(
            groupName, groupNode, parser,
            mCultures, mCultureGroups,
            cultureHandlers, cultureGroupHandlers,
            [](sCulture& c, const std::string& gid) { c.cultureGroupID = gid; }
        );
    }
}

void CultRelContainer::loadCultureData(const std::vector<fs::path>& filePaths) {

    auto start = std::chrono::high_resolution_clock::now();
    for (auto& filePath : filePaths) {
        Eu4MainParser parser;
        parser.parseFile(filePath.string());
    
        loadCultureData(parser.rootNode, parser);

    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "pegtl culture parsing and data init : " << elapsed << std::endl;
}


// all those ifs are the problem with loading with unknown values and juggling between numerical and string keys
void CultRelContainer::loadReligionData(const std::string& religionFilePath) {
    auto start = std::chrono::high_resolution_clock::now();
    Eu4MainParser parser;
    parser.parseFile(religionFilePath);

    const Node& root = parser.rootNode;

    for (const auto& [groupID, groupNode] : root.children) {
        std::string groupName = parser.intToStringID[groupID];
        for (const auto& [key, value] : groupNode.children) {
            std::string keyName = parser.intToStringID.at(key);
            if (keyName == "defender_of_faith") {
                mReligionGroups[groupName].defender_of_faith = value.value;
            }
            else if (keyName == "can_form_personal_unions") {
                mReligionGroups[groupName].can_form_personal_unions = value.value;
            }
            else if (keyName == "ai_will_propagate_through_trade") {
                mReligionGroups[groupName].ai_will_propagate_through_trade = value.value;
            }
            else if (keyName == "center_of_religion") {
                mReligionGroups[groupName].center_of_religion = std::stoi(value.value);
            }
            else if (keyName == "flags_with_emblem_percentage") {
                mReligionGroups[groupName].flags_with_emblem_percentage = std::stoi(value.value);
            }
            else if (keyName == "flag_emblem_index_range") {
                for (const auto& [flagKey, flagValue] : value.children) {
                mReligionGroups[groupName].flag_emblem_index_range.push_back(std::stoi(parser.intToStringID.at(flagKey)));
                }
            }
            else if (keyName == "harmonized_modifier") {
                mReligionGroups[groupName].harmonized_modifier = value.value;
            }
            else if (keyName == "crusade_name") {
                mReligionGroups[groupName].crusade_name = value.value;
            }
            else if(value.children.size() > 4 && keyName != "religious_schools") {
                //handle single culture
                mReligions[keyName];
                /*loadReligionDataHelper(keyName, value, parser);*/
            }
        }
        



        // REVISE ONE DAY
       /* processGroupRecursive<sReligionGroup, sReligion>(
            groupName, groupNode, parser,
            mReligions, mReligionGroups,
            religionHandlers, religionGroupHandlers,
            [](sReligion& r, const std::string& gid) { r.religionGroupID = gid; }
        );*/
    }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "pegtl religion parsing and data init : " << elapsed << std::endl;
}

void CultRelContainer::initData(FilePathHandler*& filePathHandler)
{
    initDataCulture(filePathHandler);
}

void CultRelContainer::initDataCulture(FilePathHandler*& filePathHandler)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Initiate Culture Data from file\t----\t";

    // TODO will need to change at 0 to a loop for all of the culture files but works for now with vanilla might need to change the filePathHandler to get correct ones
    // or not if i properly set it up which is possible
    const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::CULTURES_).at(0).string();
    mCultureData = new DM::FileData<CultRelContainer>(path, filePathHandler->getExportFromFullPath(path));
    mCultureData->initData(CultRelContainer::initHelperCulture, this, CultRelContainer::resetCulture);


    auto time_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
    std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;

    std::cout << mCultureGroupstest.size() << std::endl;

    for (int i = 0; i < mCulturestest.size(); ++i) {
        std::cout << mCultureData->mDataTokens[mCulturestest[i].mGroupID].getOriginName() << "\t\t:\t\t" << mCultureData->mDataTokens[mCulturestest[i].mNameID].getOriginName() << '\n';
    }
    std::cout << std::endl;

}

void CultRelContainer::initHelperCulture(DM::FileData<CultRelContainer>& fileData, CultRelContainer& cultRelContainer)
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
            //++ptr;
            //while (*ptr != '}') {
            //    switch (*ptr) {
            //    case'#': {
            //        const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
            //        ptr = commentEnd ? commentEnd : end;
            //    }
            //    case '\n':
            //    case '\t':
            //    case ' ':
            //        break;
            //    default: {
            //        /*keyStart = ptr;
            //        ptr += strcspn(ptr, "\n \t#=");
            //        Eu4::Continent& continent = GeoPolData.mContinents.back();
            //        uint16_t value;
            //        std::from_chars(keyStart, ptr, value);
            //        continent.mGeoPolIDs.push_back(value);
            //        keyStart = nullptr;*/
            //        uint16_t* value = nullptr;
            //       

            //    }
            //    ++ptr;
            //}
            //CultRelContainer::parserSkipBracket(ptr, end);
            ++ptr;
            while (*ptr != '}') {
                while (ptr < end && (*ptr < 'a' || *ptr > 'z')) ++ptr;
                char c = *ptr;
                //graphical_culture
                if (c == 'g' && *(ptr + 16) == 'e') {
                    ptr += 17;
                    while (ptr < end && *ptr != '\n') ++ptr;
                }
                //second_graphical_culture
                else if (c == 's' && *(ptr + 23) == 'e') {
                    ptr += 24;
                    while (ptr < end && *ptr != '\n') ++ptr;
                }
                //male_names
                else if (c == 'm' && *(ptr + 9) == 's') {
                    ptr += 10;
                    while (ptr < end && *ptr != '{') ++ptr;
                    CultRelContainer::parserSkipBracket(ptr, end);
                }
                //female_names
                else if (c == 'f' && *(ptr + 11) == 's') {
                    ptr += 12;
                    while (ptr < end && *ptr != '{') ++ptr;
                    CultRelContainer::parserSkipBracket(ptr, end);
                }
                //dynasty_names
                else if (c == 'd' && *(ptr + 12) == 's') {
                    ptr += 13;
                    while (ptr < end && *ptr != '{') ++ptr;
                    CultRelContainer::parserSkipBracket(ptr, end);
                }
                else {
                    keyStart = ptr;
                    ptr += strcspn(ptr, "\n \t#=");
                    cultRelContainer.mCulturestest.emplace_back(Eu4::Culture());
                    Eu4::Culture& cult = cultRelContainer.mCulturestest.back();
                    fileData.mDataTokens.emplace_back(DM::DataToken());
                    fileData.mDataTokens.back().mPtrStart = keyStart;
                    fileData.mDataTokens.back().mLength = ptr - keyStart;
                    cult.mNameID = fileData.mDataTokens.size() - 1;
                    cult.mGroupID = cultRelContainer.mCultureGroupstest.back().mNameID;
                    keyStart = nullptr;
                    ptr += strcspn(ptr, "{");
                    CultRelContainer::parserSkipBracket(ptr, end);
                }
                ++ptr;
            }
            break;
        }
        case '\n':
        case '\t':
        case ' ':
        case '=': {
            break;
        }
        default: {
            keyStart = ptr;
            while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
                ++ptr;
            }
            cultRelContainer.mCultureGroupstest.emplace_back(Eu4::CultureGroup());
            Eu4::CultureGroup& cultGroup = cultRelContainer.mCultureGroupstest.back();
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

void CultRelContainer::resetCulture(CultRelContainer& cultRelContainer)
{
}

void CultRelContainer::parserSkipBracket(const char*& ptr, const char* end)
{
   
    if (ptr >= end || *ptr != '{') return;
    ++ptr;

    int depth = 1;
    while (ptr < end && depth >0) {
        if (*ptr == '{') {
            ++depth;
        }
        else if (*ptr == '}') {
            --depth;
        }
        ++ptr;
    }
}

void CultRelContainer::loadReligionDataHelper(const std::string& religionName, const Node& node, const Eu4MainParser& parser)
{
    for (const auto& [nodeID, childNode] : node.children) {
        std::string nodeName = parser.intToStringID.at(nodeID);


        // POTENTIAL BUG since its not ordered it might actually fail it will def fail
        if (nodeName == "color") {

        }

        
    }
}


