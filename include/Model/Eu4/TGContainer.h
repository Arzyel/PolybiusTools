#pragma once
#ifndef TG_CONTAINER_H
#define TG_CONTAINER_H
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <filesystem>
#include <string_view>
#include "FileManager.h"
#include "filenfolder_CONST.h"
#include "FilePathHandler.h"

namespace Eu4 {
    class TradeGood {
    public:
        TradeGood() = default;
        ~TradeGood() = default;

        uint16_t mNameID;
        uint16_t mColorRID;
        uint16_t mColorGID;
        uint16_t mColorBID;
        uint16_t mModifierID;
        uint16_t mProvinceModID;
        uint16_t mChanceID;
        uint16_t mIsLatentID;
        uint16_t mIsValuableID;
        uint16_t mRNWChanceID;

    protected:

    private:

    };

    class TGContainer {
    public:
        TGContainer() = default;
        ~TGContainer();

        void initData(FilePathHandler*& filePathHandler);
        const std::vector<std::string_view> getAllTradeGoods() const;
        static void parserSkipUntilValueStd(const char*& ptr, std::vector<DM::DataToken>& dataTokens);

    protected:

    private:
        // might need another structure for ease of access when implementing trade good modification
        DM::FileData<TGContainer>* mTGData = nullptr;
        std::vector<TradeGood> mTradeGoods;


        static void parserSkipBracket(const char*& ptr, const char* end);

        void initDataTradeGood(FilePathHandler*& filePathHandler);
        static void initHelperTradeGood(DM::FileData<TGContainer>& fileData, TGContainer& tradeGoodContainer);
        static void resetTradeGood(TGContainer& tradeGoodContainer);

    };

}

#endif // TG_CONTAINER_H
