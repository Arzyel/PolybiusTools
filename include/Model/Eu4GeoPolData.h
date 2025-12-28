#pragma once
#ifndef EU4_GEO_POL_DATA_H
#define EU4_GEO_POL_DATA_H

#include <unordered_map>
#include <stdint.h>
#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <iostream>
#include <execution>
#include <vector>
#include <chrono>
#include <array>
#include "SimpleParser.h"
#include "filenfolder_CONST.h"
#include "Eu4GeoPolUnits.h"
#include "FilePathHandler.h"
#include "MemoryMappingFile.h"
#include "FileManager.h"

struct InformationBuffer {
	std::unordered_map<uint16_t, Eu4::Province> mProvinces;
};

struct defaultMapInfo {
	uint16_t maxProvinces;
	std::vector<uint16_t> seaStarts;
	std::vector<uint16_t> lakes;
};



namespace Eu4 {
	class GeoPolData {
	public:
		GeoPolData() = default;
		GeoPolData(const Eu4::GeoPolData&) = default;
		GeoPolData& operator=(const Eu4::GeoPolData&) = default;
		~GeoPolData();

		void fillColorToID();
		uint16_t getIDFromColor(uint32_t packedRGB) const;
		void initData(FilePathHandler*& filePathHandler);
		const Eu4::Province& getProvinceData(const int& UID) const;
		Eu4::Province& getProvinceData(const int& UID);
		const std::array<int,5> getNumberPerType() const;
		const std::vector<Eu4::Province>& getAllProvinces() const;
		const int getNbAreas() const;
		static void initHelperProvince(DM::FileData<Eu4::Province>& fileData, Eu4::Province& prov);
		static void parserSkipUntilValueStd(const char*& ptr, std::vector<DM::DataToken>& dataTokens);
		static void parserCaptureAllValuesBracket(const char*& ptr, std::vector<DM::DataToken>& dataTokens, std::vector<uint16_t>& container);
		static void parserCaptureCapital(const char*& ptr, std::vector<DM::DataToken>& dataTokens, uint16_t& capital);

	private:
		std::unordered_map<uint32_t, uint16_t> mProvColorToUID;
		std::unordered_map<uint16_t, uint32_t> mProvUIDToColor;
		std::vector<Eu4::Province> mProvinces;
		defaultMapInfo mMapInfo;
		InformationBuffer mInformationBuffer;
		std::vector<Eu4::Area> mAreas;
		std::unordered_map<std::string, uint16_t> mAreasNameToArea;
		uint16_t mAreaDataID;
		std::vector<Eu4::Region> mRegions;
		std::unordered_map<std::string, uint16_t> mRegionNameToIndex;
		uint16_t mRegionDataID;
		std::vector<Eu4::SuperRegion> mSuperRegions;
		std::unordered_map<std::string, uint16_t> mSuperRegionNameToIndex;
		uint16_t mSuperRegionDataID;
		std::vector<Eu4::Continent> mContinents;
		std::unordered_map<std::string, uint16_t> mContinentNameToIndex;
		uint16_t mContinentDataID;
		DM::FileData<Eu4::GeoPolData>* mAreasData = nullptr;
		DM::FileData<Eu4::GeoPolData>* mRegionsData = nullptr;
		DM::FileData<Eu4::GeoPolData>* mSuperRegionsData = nullptr;
		DM::FileData<Eu4::GeoPolData>* mContinentsData = nullptr;

		void initDataProvinces(FilePathHandler*& filePathHandler);
		void initMapInfo(FilePathHandler*& filePathHandler);
		void initDataAreas(FilePathHandler*& filePathHandler);
		void initDataRegions(FilePathHandler*& filePathHandler);
		void initDataSuperRegions(FilePathHandler*& filePathHandler);
		void initDataContinents(FilePathHandler*& filePathHandler);
		static void initHelperArea(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData);
		static void initHelperRegion(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData);
		static void initHelperSuperRegion(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData);
		static void initHelperContinent(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData);
		static void resetProvince(Eu4::Province& prov);
		static void resetAreas(Eu4::GeoPolData& data);
		static void resetRegions(Eu4::GeoPolData& data);
		static void resetSuperRegions(Eu4::GeoPolData& data);
		static void resetContinents(Eu4::GeoPolData& data);		
	};



}


#endif // !EU4_GEO_POL_DATA_H
