#pragma once
#ifndef COUNTRY_CONTAINER_H
#define COUNTRY_CONTAINER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <array>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <chrono>
#include "SimpleParser.h"
#include "FilePathHandler.h"
#include "FileManager.h"


namespace Eu4 {
    class Country {
    public:
        Country() = default;
        Country(std::string& path) {
            mPathTest = std::move(path);
        };
        Country(const Eu4::Country&) = default;
        Country& operator=(const Eu4::Country&) = default;
        ~Country();

        void initHistory(const std::string& path, const std::string& exportPath);

    private:
        // History file data
        uint16_t mGovernmentID;
        uint16_t mPrimaryCultureID;
        uint16_t mReligionID;
        uint16_t mTechnologyGroupID;
        uint16_t mCapitalID;
        uint16_t mSetCountryFlagID;
        std::vector<uint16_t> mAddGovReformIDs;
        std::vector<uint16_t> mAddAcceptedCultureIDs;
        std::vector<uint16_t> mAddHistoricalFriendIDs;
        std::vector<uint16_t> mAddHistoricalRivalIDs;
        DM::FileData<Eu4::Country>* mHistoryFileData = nullptr;


        //Common file data
        uint16_t mGraphicalCultureID;
        uint16_t mRID;
        uint16_t mGID;
        uint16_t mBID;
        std::array<uint8_t, 3> mRGB;
        uint32_t mpackedRGB; // might not use it to be determined
        uint16_t mRevRID;
        uint16_t mRevGID;
        uint16_t mRevBID;
        //std::vector<uint16_t> mHistIdeaGroups;
        //std::vector<uint16_t> mHistoricalUnits;
        // monarch_names
        //leader_names
        //ship_names
        //army_names
        //fleet_names
        DM::FileData<Eu4::Country>* mCommonFileData = nullptr;
        std::string mPathTest;

        static void initHelperHistoryData(DM::FileData<Eu4::Country>& fileData, Eu4::Country& countryData);
        static void initHelperCommonData(DM::FileData<Eu4::Country>& fileData, Eu4::Country& countryData);
        static void resetHistoryData(Eu4::Country& countryData);
        static void resetCommonData(Eu4::Country& countryData);

    };
}

struct CommonData {

};

class CountryContainer {

public:
    CountryContainer() = default;
    ~CountryContainer() = default;
    void initializeData(FilePathHandler*& filePathHandler);
    std::unordered_map<std::string, std::string> tagToName;
    std::unordered_map<std::string, uint16_t> tagToIndex;

    std::vector<std::string> mNames;
    std::vector<std::string> mFolderCommonKeys;
    std::vector<std::string> mFolderHistoryKeys;

protected:

private:
    std::vector<Eu4::Country> mCountriesData;

    void initHistoryData(FilePathHandler*& filePathHandler);
    void initCommonData(FilePathHandler*& filePathHandler);

};

#endif // COUNTRY_CONTAINER_H
