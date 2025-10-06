#include "FilePathHandler.h"



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

    //check optimisation in notes. basically create a bitmask to do bit operator & once 
    while (!(*ptr >= 'A' && *ptr <= 'Z') &&
        !(*ptr >= '0' && *ptr <= '9')) {

        if (*ptr == separator) {
            validatedRelative = ptr;
        }

        --ptr;
    }

    return fs::path(validatedRelative);

}

void FilePathHandler::addFilesFromFolder(const fs::path& folderPath)
{
    std::vector<fs::path> items;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            items.emplace_back(entry.path());
        }
    }

    std::ranges::sort(items);

    for (const auto& filePath : items) {
        addPath(filePath);
    }
}

void FilePathHandler::addPath(const fs::path& path)
{

    if (!mPathToIndex.empty() && mPathToIndex.contains(path.filename())) {
        mAbsolutePaths.at(mPathToIndex.at(path.filename())) = path;
    }
    else {
        mAbsolutePaths.emplace_back(path);
        mPathToIndex[path.filename()] = mAbsolutePaths.size() - 1;
    }
}

void FilePathHandler::removePath(const fs::path& path)
{
}

FilePathHandler FilePathHandlerFactory::createFPH(const std::string& game, const std::string& root, const std::string& rootExport)
{
    try {
        if (game == "eu4") return create_eu4(root, rootExport);
        if (game == "hoi4") return create_hoi4(root, rootExport);
        if (game == "ck3") return create_ck3(root, rootExport);
        throw std::runtime_error(std::string("Invalid game choice : " + game));
    }
    catch(const std::exception& e){
        std::cerr << "FilePathHandlerFactory CRITICAL: " << e.what() << std::endl;
        std::abort();
    }
    catch(...) {
        std::cerr << "FilePathHandlerFactory CRITICAL: Unknown exception" << std::endl;
        std::abort();
    }

}

FilePathHandler FilePathHandlerFactory::create_eu4(const std::string& root, const std::string& rootExport)
{
    try {
        if (!fs::exists(root)) {
            throw std::runtime_error("Root directory does not exist: " + root);
        }

        FilePathHandler handler1(root, rootExport,
            {
            relative_path::eu4::common::BUILDINGS_,
            relative_path::eu4::common::CULTURES_,
            relative_path::eu4::common::RELIGIONS_,
            relative_path::eu4::common::TRADE_GOODS_,
            relative_path::eu4::common::TRADE_NODES_,
            },
            [](const std::filesystem::path& path) -> bool {
                // TODO add logic
                std::string pathStr = path.string();
                return true;
            },
            [](const std::filesystem::path& path) -> bool {
                // TODO add logic
                std::string pathStr = path.string();
                return true; 
            });

        return handler1;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to create Eu4 handler : ") + e.what());
    }
}

FilePathHandler FilePathHandlerFactory::create_hoi4(const std::string& root, const std::string& rootExport)
{
    throw std::runtime_error("Hoi4 is not implemented");
    try {
    }
    catch (const std::exception& e) {

    }
}

FilePathHandler FilePathHandlerFactory::create_ck3(const std::string& root, const std::string& rootExport)
{
    try {
        throw std::runtime_error("Ck3 is not implemented");
    }
    catch (const std::exception& e) {

    }
}
