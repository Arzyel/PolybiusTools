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


class GeoLocation {};
class Province {};//: public GeoLocation;
class Area {};
class Region {};
class SuperRegion {};
class Continents {};

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

#endif // !EU4_GEO_POL_DATA_H
