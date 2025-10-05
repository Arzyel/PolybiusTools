#pragma once
#ifndef FILE_PATH_HANDLER_H
#define FILE_PATH_HANDLER_H
#include <filesystem>
#include <functional>
#include <vector>
#include <string>
#include <expected>
#include <unordered_map>


// TEMP KEYS

// Constants ending with "_" are folders paths otherwise they are files.

namespace games_names {
    constexpr const char* EU5 = R"(Europa Universalis V)";
    constexpr const char* EU4 = R"(Europa Universalis IV)";
    constexpr const char* HOI4 = R"(Hearts of Iron IV)";
    constexpr const char* CK3 = R"(Crusader Kings III)";
}


namespace relative_path {
    namespace eu4 {
        namespace common {
            constexpr const char* BUILDINGS_ = R"(\common\countries)";
            constexpr const char* COLONIAL_REGION_ = R"(\common\countries)";
            constexpr const char* COUNTRIES_ = R"(\common\countries)";
            constexpr const char* COUNTRY_TAGS_ = R"(\common\countries)";
            constexpr const char* CULTURES_ = R"(\common\countries)";
            constexpr const char* DISASTERS_ = R"(\common\countries)";
            constexpr const char* EVENT_MODIFIERS_ = R"(\common\countries)";
            constexpr const char* GOVERNMENT_MECHANICS_ = R"(\common\countries)";
            constexpr const char* GOVERNMENT_REFORMS_ = R"(\common\countries)";
            constexpr const char* GOVERNMENTS_ = R"(\common\countries)";
            constexpr const char* GREAT_PROJECTS_ = R"(\common\countries)";
            constexpr const char* IDEAS_ = R"(\common\countries)";
            constexpr const char* OPINION_MODIFIERS_ = R"(\common\countries)";
            constexpr const char* PROVINCE_TRIGGERED_MODIFIER_ = R"(\common\countries)";
            constexpr const char* RELIGIONS_ = R"(\common\countries)";
            constexpr const char* SUBJECT_TYPES_ = R"(\common\countries)";
            constexpr const char* TECHNOLOGIES_ = R"(\common\countries)";
            constexpr const char* TIMED_MODIFIERS_ = R"(\common\countries)";
            constexpr const char* TRADE_GOODS_ = R"(\common\countries)";
            constexpr const char* TRADE_NODES_ = R"(\common\countries)";
            constexpr const char* TRIGGERED_MODIFIERS_ = R"(\common\countries)";
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

struct FileReadError {
    enum Code {InvalidRoot, MissingFiles, PermissionDenied} code;
    std::string message;
    std::vector<std::string> missingFiles;
};


class FilePathHandlerFactory;

class FilePathHandler {
public:
    ~FilePathHandler() = default;
    std::string getFullPath(const char* relative) const;
    std::string getFullPath(size_t index) const;
    bool check(const char* relative) const;
    bool check(size_t index) const;
    bool checkAll() const;




private:
    FilePathHandler() = delete;
    FilePathHandler(const FilePathHandler&) = delete;
    FilePathHandler& operator=(const FilePathHandler&) = delete;
    FilePathHandler(FilePathHandler&&) = default;
    FilePathHandler& operator=(FilePathHandler&&) = default;
    
    template<typename Callable>
    FilePathHandler(const std::string& root, std::vector<const char*> mRelativePaths,
        Callable validator)
        :mRoot(root), mValidator(validator)
    {

    };



    std::string mRoot;
    std::vector<const char*> mRelativePaths;
    std::function<bool(const fs::path&)> mValidator;

    

    friend class FilePathHandlerFactory;
};



class FilePathHandlerFactory {
public:
    static FilePathHandler createFPH(const std::string& game, const std::string& root);
private:
    static FilePathHandler create_eu4(const std::string& root);
    static FilePathHandler create_hoi4(const std::string& root);
    static FilePathHandler create_ck3(const std::string& root);

};

#endif // FILE_PATH_HANDLER_H
