#pragma once
#ifndef GEO_POL_UNIT_H
#define GEO_POL_UNIT_H
#include <string>
#include <cstdint>
#include <array>
#include <vector>

class GeoPolUnit {
public:
    GeoPolUnit() = default;
    virtual ~GeoPolUnit() = default;

    std::string mName;
    uint32_t mRGB;

    uint32_t packRGB(const std::array<uint8_t, 3>& rgbs) {
        mRGB = ((rgbs.at(0) << 16) | (rgbs.at(1) << 8) | rgbs.at(2));
        return mRGB;
    };
    std::array<uint8_t, 3> depackRGB() const {
        return std::array<uint8_t, 3> {
            static_cast<uint8_t>((mRGB >> 16) & 0xFF),
            static_cast<uint8_t>((mRGB >> 8) & 0xFF),
            static_cast<uint8_t>(mRGB & 0xFF)
        };
    };

    virtual void initFromFile(const std::string& filePath) = 0;
    virtual void handleKeyData(const std::vector<std::string>& keyStack, const std::string& value) = 0;


protected:

private:

};

class SGeoPolUnit : public GeoPolUnit {
public:
    SGeoPolUnit() = default;
    virtual ~SGeoPolUnit() = default;
    uint16_t mUID;
};

class NSGeoPolUnit : public GeoPolUnit {
public:
    NSGeoPolUnit() = default;
    virtual ~NSGeoPolUnit() = default;

    std::vector<uint16_t> mGeoPolIDs;
};


#endif // GEO_POL_UNIT_H
