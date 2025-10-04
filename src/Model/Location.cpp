#include "Location.h"

void Location::initFromFile(const std::string& eu4UID, const uint32_t& rgbValue, const std::string& filePath)
{
	//std::unordered_map<std::string, std::string> data = SimpleParser::parseKeyValueFile(filePath);
	//mCapital = SimpleParser::getValueOrDefault(data, "capital");
	//if (mCapital == "") {
	//	return;
	//}
	////eu4CoresID.push_back(SimpleParser::getValueOrDefault(data, "add_core"));
	//eu4OwnerID = SimpleParser::getValueOrDefault(data, "owner");
	//eu4ControllerID = SimpleParser::getValueOrDefault(data, "controller");
	//eu4CultureID = SimpleParser::getValueOrDefault(data, "culture");
	//eu4ReligionID = SimpleParser::getValueOrDefault(data, "religion");
	//
	//mDevelopment.base_tax = std::stoi(SimpleParser::getValueOrDefault(data, "base_tax"));
	//mDevelopment.base_prod = std::stoi(SimpleParser::getValueOrDefault(data, "base_production"));
	//mDevelopment.base_manpower = std::stoi(SimpleParser::getValueOrDefault(data, "base_manpower"));
	//
    auto test = std::stoi(eu4UID);
	mEu4UID = eu4UID;
	RGBValue = rgbValue;
    parseEu4File(filePath,
        [this](const std::vector<std::string>& keyStack, const std::string& value) {
            handleKeyData(keyStack, value);
        }
    );

}

uint32_t Location::packRGB(const std::array<uint8_t, 3>& rgbs)
{
	return ((rgbs.at(0) << 16) | (rgbs.at(1) << 8) | rgbs.at(2));
}

void Location::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
    uint32_t keyHash = hash_runtime(keyStack.back());
    if (keyStack.size() > 1) return;
    switch (keyHash) {
    case OWNER_HASH:
        eu4OwnerID = value;
        break;
    case CONTROLLER_HASH:
        eu4ControllerID = value;
        break;
    case ADD_CORE_HASH:
        eu4CoresID.push_back(value);
        break;
    case CAPITAL_HASH:
        mCapital = value;
        break;
    case IS_CITY_HASH:
        // handle IS_CITY
        break;
    case CULTURE_HASH:
        eu4CultureID = value;
        break;
    case RELIGION_HASH:
        eu4ReligionID = value;
        break;
    case HRE_HASH:
        // handle HRE
        break;
    case BASE_TAX_HASH:
        mDevelopment.base_tax = std::stoi(value);
        break;
    case BASE_PRODUCTION_HASH:
        mDevelopment.base_prod = std::stoi(value);
        break;
    case BASE_MANPOWER_HASH:
        mDevelopment.base_manpower = std::stoi(value);
        break;
    case TRADE_GOODS_HASH:
        // handle TRADE_GOODS
        break;
    case DISCOVERED_BY_HASH:
        // handle DISCOVERED_BY
        break;
    case CENTER_OF_TRADE_HASH:
        // handle CENTER_OF_TRADE
        break;
    case ADD_PERMANENT_PROVINCE_MODIFIER_HASH:
        // handle ADD_PERMANENT_PROVINCE_MODIFIER
        break;
    case NAME_HASH:
        // handle NAME
        break;
    case DURATION_HASH:
        // handle DURATION
        break;
    case FORT_15TH_HASH:
        // handle FORT_15TH
        break;
    case ADD_PROVINCE_TRIGGERED_MODIFIER_HASH:
        // handle ADD_PROVINCE_TRIGGERED_MODIFIER
        break;
    case EXTRA_COST_HASH:
        // handle EXTRA_COST
        break;

    default:
        // unknown key
        break;
    }

}


std::array<uint8_t, 3> Location::depackRGB(uint32_t prgb) const
{
	return std::array<uint8_t, 3> {
	static_cast<uint8_t>((prgb >> 16) & 0xFF), // R
	static_cast<uint8_t>((prgb >> 8) & 0xFF),  // G
	static_cast<uint8_t>(prgb & 0xFF)          // B
	};
}
