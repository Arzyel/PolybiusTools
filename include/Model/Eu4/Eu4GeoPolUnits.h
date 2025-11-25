#pragma once
#ifndef EU4_GEO_POL_UNITS_H
#define EU4_GEO_POL_UNITS_H
#include <string>
#include <vector>
#include <unordered_map>
#include "GeoPolUnit.h"
#include "Eu4Parser.h"
#include "KEYS.h"
#include "MemoryMappingFile.h"
#include "FileManager.h"

namespace Eu4 {
	struct Development {
		uint16_t base_tax = 0;
		uint16_t base_prod = 0;
		uint16_t base_manpower = 0;
	};
	struct Development2 {
		uint16_t base_tax = 0;
		uint16_t base_prod = 0;
		uint16_t base_manpower = 0;
	};

	class Province : public SGeoPolUnit {
	public:
		std::string mOwnerID;
		uint16_t mOwnerID2;
		std::string mControllerID;
		uint16_t mControllerID2;
		std::vector<std::string> mCoresID;
		std::vector<uint16_t> mCoresID2;
		std::string mCapital;
		uint16_t mCapital2;
		std::string mCultureID;
		uint16_t mCultureID2;
		std::string mReligionID;
		uint16_t mReligionID2;
		Development mDev;
		Development mDev2;

		uint16_t mCenterofTrade;
		uint16_t mExtraCost;
		uint16_t mFort;
		uint16_t mIsCity;
		uint16_t mIsHre;
		uint16_t mTradeGood;
		uint16_t mTribalOwner;
		uint16_t mNativeSize;
		uint16_t mNativeFerocity;
		uint16_t mNativeHostile;
		std::vector<uint16_t> mTrigMod;
		std::vector<uint16_t> mDiscoveredBy;
		std::vector<uint16_t> mLatentTradeGood;



		uint16_t mAreaID;
		uint16_t mRegionID;
		uint8_t mSuperRegionID;
		uint8_t mContinentID = 0;
		bool isWater;
		/*std::string mFilePath;*/

		Province() = default;
		Province(const Province&) = default;
		Province& operator=(const Province&) = default;
		~Province() = default;



		void initFromFile(const std::string& filePath) override;
		void initFromFile(const std::string& eu4UID, const uint32_t& rgbValue,
			const std::string& filePath, const std::string& name);
		void initFromFile2(const std::string& eu4UID, const uint32_t& rgbValue, const std::string& name, const std::string& filePath);
		void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) override;
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
}

#endif // EU4_GEO_POL_UNITS_H
