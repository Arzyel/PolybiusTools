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
        DataToken(const char* lastEntry, const std::string& newData);
        ~DataToken() = default;
        std::string getOriginName() { return std::string(mPtrStart, mPtrStart + mLength); }
        std::string getCurrentName();
        uint16_t getOrigin_uint16_t();
        uint16_t getCurrent_uint16_t();
        const char* mPtrStart = nullptr;
        uint8_t mLength = 0;
        std::string mNewData;
        bool erase = false;
    };

    class FileData {
    public:
        FileData() = default;
        FileData(const std::string& importPath, const std::string& exportPath);
        ~FileData() = default;


        template<typename T, typename Callable>
        void initData(const std::string& importPath, FilePathHandler*& filePathHandler, const Callable& callable, T& target);
        template<typename T, typename Callable>
        void initData(const Callable& callable, T& target);
        void writeIntoFile();
        void initDataBuffer(const std::string& importPath);
        std::vector<DataToken> mDataTokens;
        std::string mExportPath;
        std::string mBuffer;

        
        inline void updateDataToken(int index, const std::string& newData) { mDataTokens[index].mNewData = newData; };
        inline uint16_t createNewDataToken(const std::string& newData) {	
            // IMPORTANT MIGHT ACTUALLY CREATE DANGLING POINTER MIGHT NEED TO INSTEAD HAVE A VECTOR OF DataToken* or over reserve at init by an approx amount
            if (!mDataTokens.empty()) {
                const DM::DataToken& lastEntry = mDataTokens.back();
                const char* newEntryPtr = lastEntry.mPtrStart + lastEntry.mLength;
                mDataTokens.emplace_back(DM::DataToken(newEntryPtr, newData));
            }
            else {
                mDataTokens.emplace_back(DM::DataToken(mBuffer.data(), newData));
            }
            return mDataTokens.size() - 1;
        };
    private:
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
