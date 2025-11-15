#pragma once
#ifndef EU4_GEO_POL_DATA_H
#define EU4_GEO_POL_DATA_H

#include <unordered_map>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <execution>
#include <vector>
#include <chrono>
#include "SimpleParser.h"
#include "filenfolder_CONST.h"
#include "Eu4GeoPolUnits.h"
#include "FilePathHandler.h"
#include "MemoryMappingFile.h"

struct InformationBuffer {
	std::unordered_map<uint16_t, Eu4::Province> mProvinces;
};

//class Eu4GeoPolData {
//public:
//	void fillColorToID();
//	uint16_t getIDFromColor(uint32_t packedRGB) const;
//	void initLocationData();
//	const Location& getLocationData(const int& UID) const { return mLocations.at(UID); };
//protected:
//
//private:
//	std::unordered_map<uint32_t, uint16_t> locColorToID;
//	std::unordered_map<uint16_t, uint32_t> locIDToColor;
//	std::vector<Location> mLocations;
//	//std::vector<Province> mProvinces;
//	InformationBuffer mInformationBuffer;
//	
//};



namespace Eu4 {
	class GeoPolData {
	public:
		GeoPolData() = default;
		GeoPolData(const Eu4::GeoPolData&) = default;
		GeoPolData& operator=(const Eu4::GeoPolData&) = default;
		~GeoPolData() = default;

		void fillColorToID();
		uint16_t getIDFromColor(uint32_t packedRGB) const;
		void initData(FilePathHandler*& filePathHandler);
		const Eu4::Province& getProvinceData(const int& UID) const;

	private:
		// Probably remove the Ordered vector and instead simply use the index of the vector as an ID for each and lock it in a const vector.
		// The modifying of these should create temp buffers to work with that are simply added after like continent = {0,1,2} buffercontinent = {3,4,5}
		// this way you keep the order and switch between them.
		std::unordered_map<uint32_t, uint16_t> mProvColorToUID;
		std::unordered_map<uint16_t, uint32_t> mProvUIDToColor;
		std::vector<Eu4::Province> mProvinces;
		InformationBuffer mInformationBuffer;
		std::vector<Eu4::Area> mAreas;
		std::unordered_map<std::string, uint16_t> mAreasNameToArea;
		std::vector<Eu4::Region> mRegions;
		std::unordered_map<std::string, uint16_t> mRegionNameToIndex;
		std::vector<Eu4::SuperRegion> mSuperRegions;
		std::vector<uint16_t> mOrderedSuperRegionsIDs;
		std::vector<Eu4::Continent> mContinents;
		std::vector<uint8_t> mOrderedContinentsIDs;

		void initDataProvinces();
		void initDataAreas(FilePathHandler*& filePathHandler);
		void initDataRegions(const std::string& filePath);
		void initDataSuperRegions(const std::string& filePath);
		void initDataContinents(const std::string& filePath);

		template<typename T>
		void helperReadData(const std::string& filePath, std::vector<T>& data, std::unordered_map<std::string, uint16_t>& nameToGPDUnit);
	};

	template<typename T>
	inline void GeoPolData::helperReadData(const std::string& filePath, std::vector<T>& data, std::unordered_map<std::string, uint16_t>& nameToGPDUnit)
	{
		mmap::Handle handle;
		if (!mmap::open(filePath, handle)) {
			throw std::runtime_error("Could not open file : " + filePath);
		}

		const char* ptr = handle.data;
		const char* end = ptr + handle.size;

		const char* keyStart = nullptr;
		const char* valueStart = nullptr;

		while (ptr < end) {
			char c = *ptr;
			switch (c) {
			case '#': {
				while (ptr < end && *ptr != '\n') {
					++ptr;
				}
				break;
			}
			case '{': {
				++ptr;
				while (*ptr != '}') {
					switch (*ptr) {
					case '\n':
					case '\t':
					case ' ':
						break;
					case '#': {
						while (ptr < end && *ptr != '\n') {
							++ptr;
						}
						break;
					}
					case 'c': {
						while (*ptr != '}') {
							++ptr;
						}
						++ptr;
						break;
					}
					default: {
						while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#') {
							if (valueStart == nullptr) {
								valueStart = ptr;
							}
							++ptr;

						}
						uint16_t value;
						std::from_chars(valueStart, ptr, value);
						data.back().mGeoPolIDs.push_back(value);
						valueStart = nullptr;
						--ptr;
					}
					}

					++ptr;
				}
				break;
			}
			case '\n':
			case '\t':
			case ' ':
			case '=': {
				break;
			}
			default: {
				while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
					if (keyStart == nullptr) {
						keyStart = ptr;
					}
					++ptr;
				}

				data.emplace_back(T());
				data.back().mName = std::string(keyStart, ptr);
				nameToGPDUnit.insert(std::make_pair(data.back().mName, data.size() - 1));
				keyStart = nullptr;
			}
			}
			++ptr;
		}

		mmap::close(handle);
	}
}


#endif // !EU4_GEO_POL_DATA_H
