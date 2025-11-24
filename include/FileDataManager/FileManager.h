#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include "FilePathHandler.h"

namespace DM {
    class DataToken {
    public:
        DataToken() = default;
        ~DataToken() = default;
        std::string getOriginName() { return std::string(mPtrStart, mPtrStart + mLength); }
        std::string getCurrentName();
        const char* mPtrStart = nullptr;
        uint8_t mLength = 0;
        std::string mNewData;
        bool erase = false;
    };

    class FileData {
    public:
        FileData() = default;
        ~FileData() = default;

        template<typename T, typename Callable>
        void initData(const std::string& importPath, FilePathHandler*& filePathHandler, const Callable& callable, T& target);
        void writeIntoFile();
        std::vector<DataToken> mDataTokens;
        std::string mExportPath;
        std::string mBuffer;

        
    private:
        void initDataBuffer(const std::string& importPath);
    };



    class FileManager {
    public:
        FileManager() = default;
        ~FileManager();
        std::vector<FileData*> mFiles;

    };



    template<typename T, typename Callable>
    inline void FileData::initData(const std::string& importPath, FilePathHandler*& filePathHandler, const Callable& callable, T& target)
    {
        initDataBuffer(importPath);
        mExportPath = filePathHandler->getExportFromFullPath(importPath);
        callable(*this, target);
    }

}

#endif // FILE_MANAGER_H
