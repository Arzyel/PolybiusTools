#include "Location.h"

void Location::initFromFile(const std::string& eu4UID, const uint32_t& rgbValue, const std::string& filePath)
{
	std::unordered_map<std::string, std::string> data = SimpleParser::parseKeyValueFile(filePath);
	mEu4UID = eu4UID;
	RGBValue = rgbValue;
	mCapital = SimpleParser::getValueOrDefault(data, "capital");
	//eu4CoresID.push_back(SimpleParser::getValueOrDefault(data, "add_core"));
	eu4OwnerID = SimpleParser::getValueOrDefault(data, "owner");
	eu4ControllerID = SimpleParser::getValueOrDefault(data, "controller");
	eu4CultureID = SimpleParser::getValueOrDefault(data, "culture");
	eu4ReligionID = SimpleParser::getValueOrDefault(data, "religion");


}

uint32_t Location::packRGB(const std::array<uint8_t, 3>& rgbs)
{
	return ((rgbs.at(0) << 16) | (rgbs.at(1) << 8) | rgbs.at(2));
}

std::array<uint8_t, 3> Location::depackRGB(uint32_t prgb) const
{
	return std::array<uint8_t, 3> {
	static_cast<uint8_t>((prgb >> 16) & 0xFF), // R
	static_cast<uint8_t>((prgb >> 8) & 0xFF),  // G
	static_cast<uint8_t>(prgb & 0xFF)          // B
	};
}
