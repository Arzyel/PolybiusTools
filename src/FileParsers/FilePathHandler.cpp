#include "FilePathHandler.h"


bool FilePathHandler::check(const char* relative) const
{
    return false;
}

bool FilePathHandler::check(size_t index) const
{
    return false;
}

bool FilePathHandler::checkAll() const
{
    return false;
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

FilePathHandler FilePathHandlerFactory::createFPH(const std::string& game, const std::string& root)
{
    try {
        if (game == "eu4") return create_eu4(root);
        if (game == "hoi4") return create_hoi4(root);
        if (game == "ck3") return create_ck3(root);
        throw std::runtime_error(std::string("Invalid game choice : " + game));
    }
    catch(const std::exception& e){
        std::cout << e.what();
    }

}

FilePathHandler FilePathHandlerFactory::create_eu4(const std::string& root)
{
    try {
        if (!fs::exists(root)) {
            throw std::runtime_error("Root directory does not exist: " + root);
        }

        FilePathHandler handler1(root,
            {
            relative_path::eu4::common::BUILDINGS_,
            relative_path::eu4::common::CULTURES_,
            relative_path::eu4::common::RELIGIONS_,
            relative_path::eu4::common::TRADE_GOODS_,
            relative_path::eu4::common::TRADE_NODES_,
            },
            [](const std::filesystem::path& path) -> bool {
                std::string pathStr = path.string();
                return true;
            },
            [](const std::filesystem::path& path) -> bool {
                std::string pathStr = path.string();
                return true; 
            });

        return handler1;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to create Eu4 handler : ") + e.what());
    }
}

FilePathHandler FilePathHandlerFactory::create_hoi4(const std::string& root)
{
    try {
        throw std::runtime_error("Hoi4 is not implemented");
    }
    catch (const std::exception& e) {

    }
}

FilePathHandler FilePathHandlerFactory::create_ck3(const std::string& root)
{
    try {
        throw std::runtime_error("Ck3 is not implemented");
    }
    catch (const std::exception& e) {

    }
}
