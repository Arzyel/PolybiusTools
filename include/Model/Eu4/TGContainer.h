#pragma once
#ifndef TRADE_GOOD_H
#define TRADE_GOOD_H
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

namespace fs = std::filesystem;
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

    protected:

    private:

    };

    class TGContainer {
    public:
        TGContainer() = default;
        ~TGContainer();

        void initData(FilePathHandler*& filePathHandler);
        const std::vector<std::string_view> getAllTradeGoods() const;

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

#endif // TRADE_GOOD_H
