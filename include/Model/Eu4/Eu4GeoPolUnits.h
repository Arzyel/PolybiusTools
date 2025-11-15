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

namespace Eu4 {
	struct Development {
		uint16_t base_tax;
		uint16_t base_prod;
		uint16_t base_manpower;
	};

	class Province : public SGeoPolUnit {
	public:
		std::string mOwnerID;
		std::string mControllerID;
		std::vector<std::string> mCoresID;
		std::string mCapital;
		std::string mCultureID;
		std::string mReligionID;
		Development mDev;
		uint16_t mAreaID;
		uint16_t mRegionID;
		uint8_t mSuperRegionID;
		uint8_t mContinentID;
		std::string mFilePath;

		Province() = default;
		Province(const Province&) = default;
		Province& operator=(const Province&) = default;
		~Province() = default;



		void initFromFile(const std::string& filePath) override;
		void initFromFile(const std::string& eu4UID, const uint32_t& rgbValue,
			const std::string& filePath, const std::string& name);
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
