#pragma once
#ifndef DATA_H
#define DATA_H
#include "GameData.h"
#include "CountryContainer.h"
#include "CultRelContainer.h"
#include "Eu4GeoPolData.h"
#include "FilePathHandler.h"

namespace Eu4 {
    class Data final : public GameData {
    public:
        Data(FilePathHandler*& filePathHandler);
        ~Data() = default;
        CountryContainer mCountryData;
        CultRelContainer mCultRelData;
        Eu4::GeoPolData mGeoPolData;
        //Eu4::GeoPolData mGeoPolTest;
    protected:

    private:
    };

}




#endif // DATA_H
