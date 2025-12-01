#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include "FilePathHandler.h"

namespace DM {

    class DataToken {
    public:
        DataToken() = default;
        DataToken(const char* ptrStart);
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


	class iFileDataBase {
	public:
		virtual void initDataTokens() = 0;
		virtual void resetData() = 0;
		virtual ~iFileDataBase() = default;
		std::string mExportPath;
		std::string mBuffer;
		std::vector<DataToken> mDataTokens;
		std::unordered_set<uint16_t> mActiveChanges;
		std::unordered_map<uint16_t, DataToken> mKeyToErase;

		void writeIntoFile();
		void initDataBuffer(const std::string& importPath);


		inline void clearActiveChangedData() {
			for (const uint16_t index : mActiveChanges) {
				mDataTokens[index].mNewData.clear();
			}
			mActiveChanges.clear();
		};

		inline void updateDataToken(int index, const std::string& newData) {
			mDataTokens[index].mNewData = newData;
			mDataTokens[index].erase = false;
			mActiveChanges.insert(index);
		};
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
			uint16_t id = mDataTokens.size() - 1;
			mActiveChanges.insert(id);
			return id;
		};
		inline void scheduleDelete(int index) {
			mKeyToErase.emplace(index, DataToken());
			mDataTokens[index].erase = true;
		};

		inline void clearAllTokens() {
			mDataTokens.clear();
			mActiveChanges.clear();
			mKeyToErase.clear();
		};
	};

    template<typename Target>
    class FileData : public iFileDataBase{
    public:
        FileData() = default;
        FileData(const std::string& importPath, const std::string& exportPath);
        FileData(const std::string& importPath, const std::string& exportPath, 
												void(*callable)(DM::FileData<Target>&, Target&), Target* target, 
												void(*targetResetData)(Target& target));
        ~FileData() = default;



        //void initData(const std::string& importPath, FilePathHandler*& filePathHandler, void(*callable)(DM::FileData<Target>&, Target&), Target& target);
        void initData2(void(*callable)(DM::FileData<Target>&, Target&), Target* target, void(*targetResetData)(Target& target));
        void initDataTokens() override;
        void resetData() override;

    private:
        Target* callableTarget = nullptr;
        void(*mCallinitDataTokens)(DM::FileData<Target>&, Target& target) = nullptr;
		void(*resetTargetData)(Target& target) = nullptr;

    };
}




// ------------------------ Template functions ------------------------//

template<typename Target>
DM::FileData<Target>::FileData(const std::string& importPath, const std::string& exportPath)
{
	initDataBuffer(importPath);
	mExportPath = exportPath;
}

template<typename Target>
DM::FileData<Target>::FileData(const std::string& importPath, const std::string& exportPath,
	void(*callable)(DM::FileData<Target>&, Target&), Target* target, void(*targetResetData)(Target& target))
{
	initDataBuffer(importPath);
	mExportPath = exportPath;
	mCallinitDataTokens = callable;
	callableTarget = target;
	resetTargetData = targetResetData;
}


template<typename Target>
void DM::FileData<Target>::initData2(void(*callable)(DM::FileData<Target>&, Target&), Target* target, void(*targetResetData)(Target& target))
{
	mCallinitDataTokens = callable;
	callableTarget = target;
	resetTargetData = targetResetData;
	mCallinitDataTokens(*this, *callableTarget);
}

template<typename Target>
void DM::FileData<Target>::initDataTokens()
{
	mCallinitDataTokens(*this, *callableTarget);
}

template<typename Target>
void DM::FileData<Target>::resetData()
{
	clearAllTokens();
	initDataBuffer(mExportPath);
	resetTargetData(*callableTarget);
	mCallinitDataTokens(*this, *callableTarget);
}


#endif // FILE_MANAGER_H
