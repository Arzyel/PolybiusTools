#include "Location.h"

void Location::initFromFile(const std::string& eu4UID, const uint32_t& rgbValue,
    const std::string& filePath, const std::string& name)
{
    auto test = std::stoi(eu4UID);
	mEu4UID = eu4UID;
	RGBValue = rgbValue;
    eu4ProvinceName = name;
    this->filePath = filePath;
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
        break;
    case CULTURE_HASH:
        eu4CultureID = value;
        break;
    case RELIGION_HASH:
        eu4ReligionID = value;
        break;
    case HRE_HASH:
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
        
        break;
    case DISCOVERED_BY_HASH:
        
        break;
    case CENTER_OF_TRADE_HASH:
        
        break;
    case ADD_PERMANENT_PROVINCE_MODIFIER_HASH:
        
        break;
    case NAME_HASH:
        
        break;
    case DURATION_HASH:
        
        break;
    case FORT_15TH_HASH:
        
        break;
    case ADD_PROVINCE_TRIGGERED_MODIFIER_HASH:
        
        break;
    case EXTRA_COST_HASH:
        
        break;

    default:
        
        break;
    }

}


std::array<uint8_t, 3> Location::depackRGB(uint32_t prgb) const
{
	return std::array<uint8_t, 3> {
	static_cast<uint8_t>((prgb >> 16) & 0xFF), 
	static_cast<uint8_t>((prgb >> 8) & 0xFF),  
	static_cast<uint8_t>(prgb & 0xFF)          
	};
}
