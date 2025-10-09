#include "FilePathHandler.h"





void FilePathHandler::initAllPaths()
{
    for (size_t i = 0; i < mRelativePaths.size(); i++)
    {
        const char* key = mRelativePaths.at(i);
        fs::path path = mRoot;
        path += fs::path(key);
        if (!fs::exists(path)) {
            throw std::runtime_error("Absolute Path does not exist : " + path.string());
        }

        if (fs::is_directory(path)) {
            if (fs::is_empty(path)) {
                throw std::runtime_error("No file present in folder : " + path.string());
            }
            addFilesFromFolder(path, key);
        }
        else if (fs::is_regular_file(path)) {
            addPath(path, key);
        }
        else {
            throw std::runtime_error("Unknown error at :  " + path.string());
        }
    }
}

fs::path FilePathHandler::getExportPath(const fs::path& filePath)
{
    static const auto separator = fs::path::preferred_separator;
    const auto& native_str = mAbsolutePaths.at(mPathToIndex.at(filePath)).native();
    const auto* start = native_str.c_str();
    auto size = native_str.size();
    const auto* end = start + size;
    const auto* ptr = end - 1;
    const auto* validatedRelative = end;

    while (*ptr != separator) {
        --ptr;
    }

    while (!exportRule[*ptr]) {

        if (*ptr == separator) {
            validatedRelative = ptr;
        }

        --ptr;
    }

    
    
    return mRootExport / validatedRelative;

}

void FilePathHandler::addFilesFromFolder(const fs::path& folderPath, const char* folderKey)
{
    std::vector<fs::path> items;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            items.emplace_back(entry.path());
        }
    }

    std::ranges::sort(items);

    for (const auto& filePath : items) {
        addPath(filePath, folderKey);
    }
}

void FilePathHandler::addPath(const fs::path& path, const char* folderKey)
{

    if (!mPathToIndex.empty() && mPathToIndex.contains(path.filename())) {
        mAbsolutePaths.at(mPathToIndex.at(path.filename())) = path;
    }
    else {
        mAbsolutePaths.emplace_back(path);
        mPathToIndex[path.filename()] = mAbsolutePaths.size() - 1;
        mOrderedByFolder[folderKey].push_back(mAbsolutePaths.size() - 1);
    }
}

void FilePathHandler::removePath(const fs::path& path)
{
}

FilePathHandler* FilePathHandlerFactory::createFPH(const std::string& game, const std::string& root, const std::string& rootExport)
{
    if (game == GAMES[0]) return create_eu4(root, rootExport);
    if (game == GAMES[2]) return create_hoi4(root, rootExport);
    if (game == GAMES[3]) return create_ck3(root, rootExport);
    throw std::runtime_error(std::string("Invalid game choice : " + game));


}

FilePathHandler* FilePathHandlerFactory::create_eu4(const std::string& root, const std::string& rootExport)
{
    try {
        if (!fs::exists(root)) {
            throw std::runtime_error("Root directory does not exist: " + root);
        }

        FilePathHandler* handler1 = new FilePathHandler(root, rootExport,
            {
            relative_path::eu4::common::BUILDINGS_,
            relative_path::eu4::common::CULTURES_,
            relative_path::eu4::common::RELIGIONS_,
            relative_path::eu4::common::TRADE_GOODS_,
            relative_path::eu4::common::TRADE_NODES_,
            relative_path::eu4::common::COUNTRY_TAGS_,
            relative_path::eu4::history::PROVINCES_,
            relative_path::eu4::history::COUNTRIES_,
            relative_path::eu4::map::ADJACENCIES,
            relative_path::eu4::map::AREA,
            relative_path::eu4::map::CLIMATE,
            relative_path::eu4::map::CONTINENT,
            relative_path::eu4::map::DEFAULT,
            relative_path::eu4::map::DEFINITION,
            relative_path::eu4::map::POSITIONS,
            relative_path::eu4::map::PROVINCEGROUP,
            relative_path::eu4::map::PROVINCES,
            relative_path::eu4::map::REGION,
            relative_path::eu4::map::SEASONS,
            relative_path::eu4::map::SUPERREGION,
            relative_path::eu4::map::TERRAIN_TXT,
            //relative_path::eu4::map::TERRAIN_BMP,
            //relative_path::eu4::map::TRADE_WINDS,
            //relative_path::eu4::map::TREES,
            //relative_path::eu4::map::WORLD_NORMAL,
             });

        return handler1;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to create Eu4 handler : ") + e.what());
    }
}

FilePathHandler* FilePathHandlerFactory::create_hoi4(const std::string& root, const std::string& rootExport)
{
    throw std::runtime_error("Hoi4 is not implemented");
    try {
    }
    catch (const std::exception& e) {

    }
}

FilePathHandler* FilePathHandlerFactory::create_ck3(const std::string& root, const std::string& rootExport)
{
    try {
        throw std::runtime_error("Ck3 is not implemented");
    }
    catch (const std::exception& e) {

    }
}
