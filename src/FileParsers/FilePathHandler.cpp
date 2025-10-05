#include "FilePathHandler.h"

std::string FilePathHandler::getFullPath(const char* relative) const
{
    return mRoot + relative;
}

std::string FilePathHandler::getFullPath(size_t index) const
{
    return std::string();
}

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

FilePathHandler FilePathHandlerFactory::createFPH(const std::string& game, const std::string& root)
{
    try {
        if (game == "eu4") return create_eu4(root);
        if (game == "hoi4") return create_hoi4(root);
        if (game == "ck3") return create_ck3(root);
    }
    catch(const std::exception& e){

    }
    throw std::runtime_error(std::string("Invalid game choice : " + game));

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
            [](const std::string& path) {

                if (!fs::exists(path)) {
                    throw std::runtime_error("Root directory does not exist: " + path);
                }



            });

        
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
