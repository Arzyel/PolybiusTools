#pragma once
#ifndef FILE_PATH_HANDLER_H
#define FILE_PATH_HANDLER_H
#include <filesystem>
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <ranges>
#include <array>
#include <fstream>

#include "KEYS.h"
// TEMP KEYS

// Constants ending with "_" are folders paths otherwise they are files.

constexpr const uint32_t NB_FILES = 65000;

consteval std::array<bool, 256> setupRule() {
    std::array<bool, 256> rule{};
    rule['A'] = true;

    for (size_t i = 0; i < 256; i++)
    {
        if ((i >= 'A' && i <= 'Z') ||
            (i >= '0' && i <= '9')) {
            rule[i] = true;
        }
    }

    return rule;
}

constexpr std::array<bool, 256> exportRule = setupRule();


namespace games_names {
    constexpr const char* EU5 = R"(Europa Universalis V)";
    constexpr const char* EU4 = R"(Europa Universalis IV)";
    constexpr const char* HOI4 = R"(Hearts of Iron IV)";
    constexpr const char* CK3 = R"(Crusader Kings III)";
}


namespace relative_path {
    constexpr const char* STARTUP = R"(init\startup.ini)";
    constexpr const char* OPTIONS = R"(init\options.ini)";
    namespace eu4 {
        namespace common {
            constexpr const char* BUILDINGS_ = R"(\common\buildings)";
            constexpr const char* COLONIAL_REGION_ = R"(\common\colonial_region)";
            constexpr const char* COUNTRIES_ = R"(\common\countries)";
            constexpr const char* COUNTRY_TAGS_ = R"(\common\country_tags)";
            constexpr const char* CULTURES_ = R"(\common\cultures)";
            constexpr const char* DISASTERS_ = R"(\common\disasters)";
            constexpr const char* EVENT_MODIFIERS_ = R"(\common\event_modifiers)";
            constexpr const char* GOVERNMENT_MECHANICS_ = R"(\common\government_mechanics)";
            constexpr const char* GOVERNMENT_REFORMS_ = R"(\common\government_reforms)";
            constexpr const char* GOVERNMENTS_ = R"(\common\governments)";
            constexpr const char* GREAT_PROJECTS_ = R"(\common\great_projects)";
            constexpr const char* IDEAS_ = R"(\common\ideas)";
            constexpr const char* OPINION_MODIFIERS_ = R"(\common\opinion_modifiers)";
            constexpr const char* PROVINCE_TRIGGERED_MODIFIERS_ = R"(\common\province_triggered_modifiers)";
            constexpr const char* RELIGIONS_ = R"(\common\religions)";
            constexpr const char* SUBJECT_TYPES_ = R"(\common\subject_types)";
            constexpr const char* TECHNOLOGIES_ = R"(\common\technologies)";
            constexpr const char* TIMED_MODIFIERS_ = R"(\common\timed_modifiers)";
            constexpr const char* TRADE_GOODS_ = R"(\common\tradegoods)";
            constexpr const char* TRADE_NODES_ = R"(\common\tradenodes)";
            constexpr const char* TRIGGERED_MODIFIERS_ = R"(\common\triggered_modifiers)";
        }

        namespace decisions {

        }

        namespace events {

        }

        namespace gfx {

        }

        namespace history {
            constexpr const char* ADVISORS_ = R"(\history\advisors)";
            constexpr const char* COUNTRIES_ = R"(\history\countries)";
            constexpr const char* DIPLOMACY_ = R"(\history\diplomacy)";
            constexpr const char* PROVINCES_ = R"(\history\provinces)";
            constexpr const char* WARS_ = R"(\history\wars)";
        }


        constexpr const char* LOCALISATION_ = R"(\localisation)";
        namespace localisation {

        }

        
        namespace map {
            namespace lakes {
                constexpr const char* LAKES = R"(\map\lakes\00_lakes.txt)";
            }
            constexpr const char* ADJACENCIES = R"(\map\adjacencies.csv)";
            constexpr const char* AREA = R"(\map\area.txt)";
            constexpr const char* CLIMATE = R"(\map\climate.txt)";
            constexpr const char* CONTINENT = R"(\map\continent.txt)";
            constexpr const char* DEFAULT = R"(\map\default.map)";
            constexpr const char* DEFINITION = R"(\map\definition.csv)";
            constexpr const char* HEIGHTMAP = R"(\map\heightmap.bmp)";
            constexpr const char* POSITIONS = R"(\map\positions.txt)";
            constexpr const char* PROVINCEGROUP = R"(\map\provincegroup.txt)";
            constexpr const char* PROVINCES = R"(\map\provinces.bmp)";
            constexpr const char* REGION = R"(\map\region.txt)";
            constexpr const char* SEASONS = R"(\map\seasons.txt)";
            constexpr const char* SUPERREGION = R"(\map\superregion.txt)";
            constexpr const char* TERRAIN_TXT = R"(\map\terrain.txt)";
            constexpr const char* TERRAIN_BMP = R"(\map\terrain.bmp)";
            constexpr const char* TRADE_WINDS = R"(\map\trade_winds.txt)";
            constexpr const char* TREES = R"(\map\trees.bmp)";
            constexpr const char* WORLD_NORMAL = R"(\map\world_normal.bmp)";
        }

        constexpr const char* MISSIONS_ = R"(\missions)";
        namespace missions {

        }
    }
}




namespace fs = std::filesystem;

struct ModFile {
    std::string name;
    std::string modPath;
};



class FilePathHandlerFactory;

class FilePathHandler {
public:
    ~FilePathHandler() = default;

    std::string mGame;

    void initAllPaths();
    std::function<void()> mDirectoryValidator;
    fs::path getExportPath(const fs::path& filePath);
    std::string getExportFromFullPath(const std::string& filePath);
    const std::vector<fs::path> getPathsFromFolderKey(const char* key) {
        std::vector<fs::path> paths;
                
        for (auto& index : mOrderedByFolder.at(key)) {
            paths.emplace_back(mAbsolutePaths.at(index));
        }
        
        return paths;
    };

    void addFilesFromFolder(const fs::path& folderPath, const char* folderKey);
    void addPath(const fs::path& path, const char* folderKey);
    void removePath(const fs::path& path);
    void initModsPath(const fs::path& path, std::vector<ModFile>& modFilesData);
    
private:
    FilePathHandler() = delete;
    FilePathHandler(const FilePathHandler&) = delete;
    FilePathHandler& operator=(const FilePathHandler&) = delete;
    FilePathHandler(FilePathHandler&&) = default;
    FilePathHandler& operator=(FilePathHandler&&) = default;
    
    FilePathHandler(const std::string& root, const std::string& rootExport,
        std::vector<const char*> relativePaths, const std::string& game)
        :mRoot(root), mRootExport(rootExport), mRelativePaths(relativePaths.begin(), relativePaths.end()), mGame(game)
    {
        mAbsolutePaths.reserve(NB_FILES);
    };


    fs::path mRoot;
    fs::path mRootExport;
    std::vector<fs::path> mAbsolutePaths;
    std::vector<const char*> mRelativePaths;
    std::unordered_map<fs::path, uint32_t> mPathToIndex;
    std::unordered_map<const char*, std::vector<uint32_t>> mOrderedByFolder;
    std::function<bool(const fs::path&)> mFileValidator;

    

    friend class FilePathHandlerFactory;
};



class FilePathHandlerFactory {
public:
    static FilePathHandler* createFPH(const std::string& game, const std::string& root, const std::string& rootExport);
private:
    static FilePathHandler* create_eu4(const std::string& game, const std::string& root, const std::string& rootExport);
    static FilePathHandler* create_hoi4(const std::string& root, const std::string& rootExport);
    static FilePathHandler* create_ck3(const std::string& root, const std::string& rootExport);

};

#endif // FILE_PATH_HANDLER_H
