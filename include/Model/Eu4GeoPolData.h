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
#include "Location.h"
#include "filenfolder_CONST.h"
#include "Eu4GeoPolUnits.h"

struct InformationBuffer {
	std::unordered_map<uint16_t, Location> mLocations;
};

class Eu4GeoPolData {
public:
	void fillColorToID();
	uint16_t getIDFromColor(uint32_t packedRGB) const;
	void initLocationData();
	const Location& getLocationData(const int& UID) const { return mLocations.at(UID); };
protected:

private:
	std::unordered_map<uint32_t, uint16_t> locColorToID;
	std::unordered_map<uint16_t, uint32_t> locIDToColor;
	std::vector<Location> mLocations;
	//std::vector<Province> mProvinces;
	InformationBuffer mInformationBuffer;
	
};



namespace Eu4 {
	class GeoPolData {
	public:
		GeoPolData() = default;
		GeoPolData(const Eu4::GeoPolData&) = default;
		GeoPolData& operator=(const Eu4::GeoPolData&) = default;
		~GeoPolData() = default;

		void fillColorToID();
		uint16_t getIDFromColor(uint32_t packedRGB) const;
		void initData();
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
		std::vector<uint16_t> mOrderedAreasIDs;
		std::vector<Eu4::Region> mRegions;
		std::vector<uint16_t> mOrderedRegionsIDs;
		std::vector<Eu4::SuperRegion> mSuperRegions;
		std::vector<uint16_t> mOrderedSuperRegionsIDs;
		std::vector<Eu4::Continent> mContinents;
		std::vector<uint8_t> mOrderedContinentsIDs;

		void initDataProvinces();
		void initDataAreas();
		void initDataRegions();
		void initDataSuperRegions();
		void initDataContinents();
	};
}


#endif // !EU4_GEO_POL_DATA_H
