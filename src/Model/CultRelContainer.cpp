// ============================================================================
// CultRelContainer.cpp
// ============================================================================
#include "CultRelContainer.h"
#include <sstream>
#include <algorithm>

CultRelContainer::CultRelContainer() {
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

void CultRelContainer::loadCultureData(const std::string& filePath) {
    Eu4MainParser parser;
    auto start = std::chrono::high_resolution_clock::now();
    parser.parseFile(filePath);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "parsing with pegtl in the old parser : " << elapsed << " mss\n" << std::endl;
    
    loadCultureData(parser.rootNode, parser);
}


// all those ifs are the problem with loading with unknown values and juggling between numerical and string keys
void CultRelContainer::loadReligionData(const std::string& religionFilePath) {
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