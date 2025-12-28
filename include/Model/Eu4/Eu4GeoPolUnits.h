#pragma once
#ifndef EU4_GEO_POL_UNITS_H
#define EU4_GEO_POL_UNITS_H
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include "GeoPolUnit.h"
#include "Eu4Parser.h"
#include "KEYS.h"
#include "MemoryMappingFile.h"
#include "FileManager.h"
#include "KEYS.h"

namespace Eu4 {
	class Province : public SGeoPolUnit {
	public:
		DM::FileData<Eu4::Province>* mFileData = nullptr;
		uint16_t mOwnerID2 = UINT16_MAX;
		uint16_t mControllerID2 = UINT16_MAX;
		std::vector<uint16_t> mCoresID2;
		uint16_t mCapital2 = UINT16_MAX;
		uint16_t mCultureID2 = UINT16_MAX;
		uint16_t mReligionID2 = UINT16_MAX;
		uint16_t mCenterofTrade = UINT16_MAX;
		uint16_t mExtraCost = UINT16_MAX;
		uint16_t mFort = UINT16_MAX;
		uint16_t mIsCity = UINT16_MAX;
		uint16_t mIsHre = UINT16_MAX;
		uint16_t mTradeGood = UINT16_MAX;
		uint16_t mTribalOwner = UINT16_MAX;
		uint16_t mNativeSize = UINT16_MAX;
		uint16_t mNativeFerocity = UINT16_MAX;
		uint16_t mNativeHostile = UINT16_MAX;
		uint16_t mBaseTax = UINT16_MAX;
		uint16_t mBaseProduction = UINT16_MAX;
		uint16_t mBaseManpower = UINT16_MAX;
		std::vector<uint16_t> mTrigMod;
		std::vector<uint16_t> mDiscoveredBy;
		std::vector<uint16_t> mLatentTradeGood;


		uint16_t mAreaID;
		uint16_t mRegionID;
		uint8_t mSuperRegionID;
		uint8_t mContinentID = 0;
		bool isWater = false;
		bool isWasteland = false;
		/*std::string mFilePath;*/

		Province() = default;
		Province(const Province&) = default;
		Province& operator=(const Province&) = default;
		~Province();


		void initFromFile(const std::string& filePath) override;
		void initFromFile(const std::string& eu4UID, const uint32_t& rgbValue,
			const std::string& filePath, const std::string& name);
		void initFromFile2(const std::string& eu4UID, const uint32_t& rgbValue, const std::string& name, const std::string& filePath);
		void initFromFile2(const uint16_t eu4UID, const uint32_t& rgbValue, const std::string& name, const std::string& filePath);
		void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) override;
		void resetData();


		void updateField(uint16_t Eu4::Province::* memberPtr, const std::string& newData);

		inline void scheduleDelete(uint16_t Eu4::Province::* memberPtr) {
			this->mFileData->scheduleDelete(this->*memberPtr);
		};
	};

	class Area : public NSGeoPolUnit {
	public:
		Area() = default;
		~Area() = default;

		void initFromFile(const std::string& filePath) override;
		void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) override;
	};

	class Region : public NSGeoPolUnit {
	public:
		Region() = default;
		~Region() = default;
		void initFromFile(const std::string& filePath) override;
		void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) override;
	};

	class SuperRegion : public NSGeoPolUnit {
	public:
		SuperRegion() = default;
		~SuperRegion() = default;
		std::string getName() { return std::string(testName.mPtrStart, testName.mPtrStart + testName.mLength);};
		void initFromFile(const std::string& filePath) override;
		void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) override;
	};

	class Continent : public NSGeoPolUnit {
	public:
		void initFromFile(const std::string& filePath) override;
		void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) override;
	};

	struct MemberPtrHash {
		std::size_t operator()(uint16_t Eu4::Province::* ptr) const noexcept {
			// Hash the raw bytes of the pointer-to-member using FNV-1a (64-bit)
			const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&ptr);
			std::uint64_t hash = 14695981039346656037ull; // FNV offset basis
			for (std::size_t i = 0; i < sizeof(ptr); ++i) {
				hash ^= static_cast<std::uint64_t>(bytes[i]);
				hash *= 1099511628211ull; // FNV prime
			}
			// Truncate to size_t
			if (sizeof(std::size_t) < sizeof(hash)) {
				return static_cast<std::size_t>(hash ^ (hash >> 32));
			}
			return static_cast<std::size_t>(hash);
		}
	};

	struct MemberPtrEq {
		template <typename T>
		bool operator()(T a, T b) const noexcept {
			return std::memcmp(&a, &b, sizeof(T)) == 0;
		}
	};

	static const std::unordered_map<uint16_t Eu4::Province::*, std::string,
	MemberPtrHash,
	MemberPtrEq> FieldMap_U16 = {
		{ &Eu4::Province::mOwnerID2,        OWNER_CHAR },
		{ &Eu4::Province::mControllerID2,   CONTROLLER_CHAR },
		{ &Eu4::Province::mCapital2,        CAPITAL_CHAR },
		{ &Eu4::Province::mCultureID2,      CULTURE_CHAR },
		{ &Eu4::Province::mReligionID2,     RELIGION_CHAR },
		{ &Eu4::Province::mCenterofTrade,   CENTER_OF_TRADE_CHAR },
		{ &Eu4::Province::mExtraCost,       EXTRA_COST_CHAR },
		{ &Eu4::Province::mFort,            FORT_15TH_CHAR },
		{ &Eu4::Province::mIsCity,          IS_CITY_CHAR },
		{ &Eu4::Province::mIsHre,           HRE_CHAR },
		{ &Eu4::Province::mTradeGood,       TRADE_GOODS_CHAR },
		{ &Eu4::Province::mTribalOwner,     TRIBAL_OWNER_CHAR },
		{ &Eu4::Province::mNativeSize,      NATIVE_SIZE_CHAR },
		{ &Eu4::Province::mNativeFerocity,  NATIVE_FEROCITY_CHAR },
		{ &Eu4::Province::mNativeHostile,   NATIVE_HOSTILENESS_CHAR },
		{ &Eu4::Province::mBaseTax,         BASE_TAX_CHAR},
		{ &Eu4::Province::mBaseProduction,  BASE_PRODUCTION_CHAR },
		{ &Eu4::Province::mBaseManpower,    BASE_MANPOWER_CHAR }
	};

}

inline void Eu4::Province::resetData()
{
	mOwnerID2 = UINT16_MAX;
	mControllerID2 = UINT16_MAX;
	mCoresID2.clear();
	mCapital2 = UINT16_MAX;
	mCultureID2 = UINT16_MAX;
	mReligionID2 = UINT16_MAX;
	mCenterofTrade = UINT16_MAX;
	mExtraCost = UINT16_MAX;
	mFort = UINT16_MAX;
	mIsCity = UINT16_MAX;
	mIsHre = UINT16_MAX;
	mTradeGood = UINT16_MAX;
	mTribalOwner = UINT16_MAX;
	mNativeSize = UINT16_MAX;
	mNativeFerocity = UINT16_MAX;
	mNativeHostile = UINT16_MAX;
	mBaseTax = UINT16_MAX;
	mBaseProduction = UINT16_MAX;
	mBaseManpower = UINT16_MAX;
	mTrigMod.clear();
	mDiscoveredBy.clear();
	mLatentTradeGood.clear();
}
inline void Eu4::Province::updateField(uint16_t Eu4::Province::* memberPtr, const std::string& newData)
{
	if ((this->*memberPtr) != UINT16_MAX) {
		this->mFileData->updateDataToken(this->*memberPtr, newData, Eu4::FieldMap_U16.at(memberPtr));
		//this->mFileData->updateDataToken(this->*memberPtr, newData);
		return;
	}
	// TODO modify method to take the string of the key too
	(this->*memberPtr) = this->mFileData->createNewDataToken(newData, Eu4::FieldMap_U16.at(memberPtr));
}






#endif // EU4_GEO_POL_UNITS_H
