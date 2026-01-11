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

std::string FilePathHandler::getExportFromFullPath(const std::string& filePath)
{
    static const auto separator = fs::path::preferred_separator;

    const auto* start = filePath.c_str();
    auto size = filePath.size();
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



    return mRootExport.string() + validatedRelative;
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


uint32_t FilePathHandler::initModsPath(const fs::path& dirPath)
{
    std::vector<fs::path> modFilePaths;
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        throw std::runtime_error("Directory does not exists : " + dirPath.string());
    }
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".mod") {
            modFilePaths.push_back(entry.path());
        }
    }


    for (const auto& modFilepath : modFilePaths) {
        modFilesData.push_back(ModFile());
        ModFile& modFileData = modFilesData.back();

        std::ifstream file(modFilepath.string(), std::ios::binary | std::ios::ate);
        if (!file) {
            throw std::runtime_error("Failed to open File : " + modFilepath.string());
        }

        const auto size = file.tellg();
        if (size == std::ifstream::pos_type(-1)) {
            throw std::runtime_error("Failed to determine file size : " + modFilepath.string());
        }
        file.seekg(0);

        std::vector<char> buffer(size);
        file.read(buffer.data(), size);

        const char* ptr = buffer.data();
        const char* end = ptr + size;

        while (ptr < end) {
            //name
            if (*ptr == 'n' && std::memcmp(ptr, "name=\"", 5) == 0) {
                ptr += 6;
                const char* start = ptr;
                while (ptr < end && *ptr != '"') ++ptr;
                modFileData.name = std::string(start, ptr);
            }
            //path
            else if (*ptr == 'p' && *(ptr - 1) == '\n' && std::memcmp(ptr, "path=\"", 5) == 0) {
                ptr += 6;
                const char* start = ptr;
                while (ptr < end && *ptr != '"') ++ptr;
                modFileData.modPath = std::string(start, ptr);
            }

            if (!modFileData.name.empty() && !modFileData.modPath.empty()) break;
            ++ptr;
        }
    }
    return modFilesData.size();
}

const std::string_view FilePathHandler::getModPath(int index)
{
    return std::string_view(modFilesData.at(index).modPath);
}

void FilePathHandler::setActiveMod(int index)
{
    if (index >= 0) mActiveModIndex = index;
    else throw std::runtime_error("Invalid mod index given");
}

void FilePathHandler::updateAPWithModsAP()
{
    fs::path modPathRoot = modFilesData.at(mActiveModIndex).modPath;

    if (!fs::exists(modPathRoot)) {
        throw std::runtime_error(std::string("Absolute Path For Mod : "+ modFilesData.at(mActiveModIndex).name + " does not exist.\nCurrent path given :" + modPathRoot.string()));
    }

    for (const auto relativePath : mRelativePaths) {
        fs::path modPath = modPathRoot;
        modPath += fs::path(relativePath);
        if (!fs::exists(modPath)) {
            std::cout << "Reconstructed mod path : " + modPath.string() + " does not exist skip to next one." << std::endl;
            continue;
        }

        if (fs::is_directory(modPath)) {
            if (fs::is_empty(modPath)) {
                std::cout << "Reconstructed mod path : " + modPath.string() + " no file found in directory skipping to next one." << std::endl;
                continue;
            }
            addFilesFromFolder(modPath, relativePath);
        }
        else if (fs::is_regular_file(modPath)) {
            addPath(modPath, relativePath);
        }
        else {
            throw std::runtime_error("Unknown error at :  " + modPath.string() + "while trying to update path for mods");
        }
    }
}


void FilePathHandler::populateProvincesFilePathStructure(
    std::vector<std::tuple<uint16_t, std::string, std::filesystem::path>>& files,
    uint32_t nbProvinces)
{
    files.clear();

    std::unordered_map<uint16_t, std::pair<std::string, std::filesystem::path>> provinceByID;
    provinceByID.reserve(nbProvinces);

    for (const auto& path : mAbsolutePaths) {
        const auto parent = path.parent_path();
        const auto grandparent = parent.parent_path();

        if (parent.filename() == "provinces" && grandparent.filename() == "history") {
            if (path.extension() != ".txt") continue;

            const std::string buffer = path.filename().string();
            const char* ptr = buffer.data();
            const char* end = ptr + buffer.size();

            uint16_t number = 0;
            while (ptr < end && *ptr >= '0' && *ptr <= '9') {
                number = number * 10 + (*ptr - '0');
                ++ptr;
            }

            while (ptr < end && (*ptr == ' ' || *ptr == '-')) ++ptr;

            const char* name_start = ptr;
            const char* name_end = end - 4;

            std::string name(name_start, name_end);
            if (provinceByID.count(number)) {
                std::cout << "Overwriting province " << number << ":\n"
                    << "  Old: " << provinceByID[number].second.string() << "\n"
                    << "  New: " << path.string() << "\n";
            }
            provinceByID[number] = { std::move(name), path };
        }

    }
    files.reserve(provinceByID.size());
    for (auto& [id, data] : provinceByID) {
        files.emplace_back(id, std::move(data.first), std::move(data.second));
    }
}

FilePathHandler* FilePathHandlerFactory::createFPH(const std::string& game, const std::string& root, const std::string& rootExport)
{
    if (game == GAMES[0]) return create_eu4(game, root, rootExport);
    if (game == GAMES[2]) return create_hoi4(root, rootExport);
    if (game == GAMES[3]) return create_ck3(root, rootExport);
    throw std::runtime_error(std::string("Invalid game choice : " + game));

}

FilePathHandler* FilePathHandlerFactory::create_eu4(const std::string& game, const std::string& root, const std::string& rootExport)
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
             }, game);

        return handler1;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to create Eu4 handler : ") + e.what());
    }
}

FilePathHandler* FilePathHandlerFactory::create_hoi4(const std::string& root, const std::string& rootExport)
{
    throw std::runtime_error("Hoi4 is not implemented");

}

FilePathHandler* FilePathHandlerFactory::create_ck3(const std::string& root, const std::string& rootExport)
{
    throw std::runtime_error("Ck3 is not implemented");

}
