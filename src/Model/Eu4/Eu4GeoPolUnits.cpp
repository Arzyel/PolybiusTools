#include "Eu4GeoPolUnits.h"

Eu4::Province::~Province()
{
    delete mFileData;
}

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


void Eu4::Province::initFromFile2(const std::string& eu4UID, const uint32_t& rgbValue, const std::string& name, const std::string& filePath)
{
    mUID = std::stoi(eu4UID);
    mRGB = rgbValue;
    mName = name;
    mFilePath = filePath;
    
}

void Eu4::Province::initFromFile2(const uint16_t eu4UID, const uint32_t& rgbValue, const std::string& name, const std::string& filePath)
{
    mUID = eu4UID;
    mRGB = rgbValue;
    mName = name;
    mFilePath = filePath;
   
}

void Eu4::Province::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
   
}

void Eu4::Area::initFromFile(const std::string& filePath)
{

}

void Eu4::Area::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{

}

void Eu4::Region::initFromFile(const std::string& filePath)
{
}

void Eu4::Region::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
}

void Eu4::SuperRegion::initFromFile(const std::string& filePath)
{
}

void Eu4::SuperRegion::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
}

void Eu4::Continent::initFromFile(const std::string& filePath)
{
}

void Eu4::Continent::handleKeyData(const std::vector<std::string>& keyStack, const std::string& value)
{
}
