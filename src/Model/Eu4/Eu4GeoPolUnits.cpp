#include "Eu4GeoPolUnits.h"

void Eu4::Province::initFromFile(const std::string& filePath)
{
}

void Eu4::Province::initFromFile(const std::string& eu4UID, const uint32_t& rgbValue, const std::string& filePath, const std::string& name)

{
    mUID = std::stoi(eu4UID);
    mRGB = rgbValue;
    mName = name;
    mFilePath = filePath;
    parseEu4File(filePath,
        [this](const std::vector<std::string>& keyStack, const std::string& value) {
            handleKeyData(keyStack, value);
        }
    );
}

void Eu4::Province::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
    uint32_t keyHash = hash_runtime(keyStack.back());
    if (keyStack.size() > 1) return;
    switch (keyHash) {
    case OWNER_HASH:
        mOwnerID = value;
        break;
    case CONTROLLER_HASH:
        mControllerID = value;
        break;
    case ADD_CORE_HASH:
        mCoresID.push_back(value);
        break;
    case CAPITAL_HASH:
        mCapital = value;
        break;
    case IS_CITY_HASH:
        break;
    case CULTURE_HASH:
        mCultureID = value;
        break;
    case RELIGION_HASH:
        mReligionID = value;
        break;
    case HRE_HASH:
        break;
    case BASE_TAX_HASH:
        mDev.base_tax = std::stoi(value);
        break;
    case BASE_PRODUCTION_HASH:
        mDev.base_prod = std::stoi(value);
        break;
    case BASE_MANPOWER_HASH:
        mDev.base_manpower = std::stoi(value);
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

void Eu4::Area::initFromFile(const std::string& filePath)
{

}

void Eu4::Area::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
    mName = keyStack.back();


}
