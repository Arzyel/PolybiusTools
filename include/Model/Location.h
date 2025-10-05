#pragma once
#ifndef LOCATION_H
#define LOCATION_H
#include <stdint.h>
#include <string>
#include <vector>
#include <array>

#include "filenfolder_CONST.h"
#include "SimpleParser.h"
#include "KEYS.h"
#include "Eu4Parser.h"

struct Development {
	uint16_t base_tax;
	uint16_t base_prod;
	uint16_t base_manpower;
};


class Location {
public:
	void initFromFile(const std::string& eu4UID, const uint32_t& rgbValue,
		const std::string& filePath, const std::string& name);
	uint32_t packRGB(const std::array<uint8_t, 3>& rgbs);
	void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value);
	std::array<uint8_t, 3> depackRGB(uint32_t prgb) const;
	uint32_t RGBValue;
	uint16_t mUID;
	std::vector<uint16_t> mCoresID;
	uint16_t mOwnerID;
	uint16_t mControllerID;
	uint16_t mCultureID;
	uint16_t mReligionID;



	//TODO REFACTOR to seperate string values from internal nb based values
	std::string filePath = "";
	std::string eu4ProvinceName = "";
	std::string mEu4UID = "";
	std::string eu4OwnerID= "";
	std::vector<std::string> eu4CoresID;
	std::string eu4ControllerID = "";
	std::string eu4CultureID = "";
	std::string eu4ReligionID = "";
	std::string eu4devTax = "";
	std::string eu4devProd = "";
	std::string eu4devMan = "";


	bool mIsHRE;
	bool mIsCity;
	Development mDevelopment;
	std::string mCapital = "";
	uint8_t mCenterTrade;
	std::vector<std::string> mDiscoveredBy;
protected:
private:
};

#endif // !LOCATION_H
