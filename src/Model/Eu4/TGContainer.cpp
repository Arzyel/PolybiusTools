#include "TGContainer.h"

Eu4::TGContainer::~TGContainer()
{
	delete mTGData;
}

const std::vector<std::string_view> Eu4::TGContainer::getAllTradeGoods() const
{
	return std::vector<std::string_view>();
}

void Eu4::TGContainer::initData(FilePathHandler*& filePathHandler)
{
	initDataTradeGood(filePathHandler);
}

void Eu4::TGContainer::initDataTradeGood(FilePathHandler*& filePathHandler)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Initiate Trade Good Data from file\t----\t:" << filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::TRADE_GOODS_).at(0).string();

    
    const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::TRADE_GOODS_).at(0).string();
    mTGData = new DM::FileData<TGContainer>(path, filePathHandler->getExportFromFullPath(path));
    mTGData->initData(TGContainer::initHelperTradeGood, this, TGContainer::resetTradeGood);


    auto time_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - start);
    std::cout << "Elapsed Time : " << elapsed.count() << "ms" << std::endl;
}

void Eu4::TGContainer::initHelperTradeGood(DM::FileData<TGContainer>& fileData, TGContainer& tradeGoodContainer)
{
    const char* ptr = fileData.mBuffer.data();
    const char* end = ptr + fileData.mBuffer.size();
    std::vector<DM::DataToken>& dataTokens = fileData.mDataTokens;


    const char* keyStart = nullptr;
    while (ptr < end) {
        switch (*ptr) {
        case '#': {
            const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
            ptr = commentEnd ? commentEnd : end;

            break;
        }
        case '{': {
            ++ptr;
            while (*ptr != '}' && ptr < end) {
            uint16_t* valueMemberPtr = nullptr;
                while (ptr < end && (*ptr < 'a' || *ptr > 'z') && *ptr != '#' && *ptr != '}') ++ptr;
                if (*ptr == '#') {
                    while (*ptr != '\n') ++ptr;
                    continue;
                }
                else if (*ptr == '}') {
                    continue;
                }
                char c = *ptr;
                bool matched = true;
                
                switch (c) {
                case'c':
                    //color
                    if (*(ptr + 4) == 'r') {
                        ptr += 5;
                        while (*ptr != '{' && ptr < end) ++ptr;
                        Eu4::TGContainer::parserSkipBracket(ptr, end);
                        break;
                    }
                    //chance
                    else if (*(ptr + 5) == 'e') {
                        ptr += 6;
                        while (*ptr != '{' && ptr < end) ++ptr;
                        Eu4::TGContainer::parserSkipBracket(ptr, end);
                    }
                    break;
                case 'i':
                    //is_latent
                    if (*(ptr + 8) == 't') {
                        ptr += 9;
                        valueMemberPtr = &(tradeGoodContainer.mTradeGoods.back().mIsLatentID);
                        break;
                    }
                    //is_valuable
                    else if (*(ptr + 10) == 'e') {
                        ptr += 11;
                        valueMemberPtr = &(tradeGoodContainer.mTradeGoods.back().mIsValuableID);
                        break;
                    }
                    break;
                case 'r':
                    //rnw_latent_chance
                    if (*(ptr + 16) == 'e') {
                        ptr += 17;
                        valueMemberPtr = &(tradeGoodContainer.mTradeGoods.back().mRNWChanceID);
                        break;
                    }
                    break;
                case 't':
                    //trigger
                    if (*(ptr + 6) == 'r') {
                        ptr += 7;
                        while (*ptr != '{' && ptr < end) ++ptr;
                        Eu4::TGContainer::parserSkipBracket(ptr, end);
                    }
                    break;
                case 'm' :
                    //modifier
                    if (*(ptr + 7) == 'r') {
                    ptr += 7;
                    while (*ptr != '{' && ptr < end) ++ptr;
                    Eu4::TGContainer::parserSkipBracket(ptr, end);
                }
                break;
                case 'p' :
                    //province
                    if (*(ptr + 7) == 'e') {
                    ptr += 7;
                    while (*ptr != '{' && ptr < end) ++ptr;
                    Eu4::TGContainer::parserSkipBracket(ptr, end);
                }
                break;
                    
                default:
                    throw new std::runtime_error(std::string("failed parsing at position " + (end - ptr)));
                    break;
                }

                if (valueMemberPtr != nullptr) {
                    Eu4::TGContainer::parserSkipUntilValueStd(ptr, dataTokens);
                    *valueMemberPtr = fileData.mDataTokens.size() - 1;
                }

                ++ptr;

            }
            break;
        }
        case '\n':
        case '\t':
        case ' ':
        case '=': {
            break;
        }
        default: {
            keyStart = ptr;
            while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
                ++ptr;
            }
            tradeGoodContainer.mTradeGoods.emplace_back(Eu4::TradeGood());
            Eu4::TradeGood& tradeGood = tradeGoodContainer.mTradeGoods.back();
            fileData.mDataTokens.emplace_back(DM::DataToken());
            fileData.mDataTokens.back().mPtrStart = keyStart;
            fileData.mDataTokens.back().mLength = ptr - keyStart;
            tradeGood.mNameID = fileData.mDataTokens.size() - 1;
            keyStart = nullptr;
        }
        }
        ++ptr;
    }
}

void Eu4::TGContainer::resetTradeGood(TGContainer& tradeGoodContainer)
{
}

void Eu4::TGContainer::parserSkipBracket(const char*& ptr, const char* end)
{

    if (ptr >= end || *ptr != '{') return;
    ++ptr;

    int depth = 1;
    while (ptr < end && depth >0) {
        if (*ptr == '{') {
            ++depth;
        }
        else if (*ptr == '}') {
            --depth;
        }
        ++ptr;
    }
}

inline void Eu4::TGContainer::parserSkipUntilValueStd(const char*& ptr, std::vector<DM::DataToken>& dataTokens)
{
    while (*ptr != '=') ++ptr;
    ++ptr;
    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\"') ++ptr;
    const char* keyStart = ptr;
    while (*ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '#' && *ptr != '\"') ++ptr;
    dataTokens.emplace_back(DM::DataToken());
    dataTokens.back().mPtrStart = keyStart;
    dataTokens.back().mLength = ptr - keyStart;
    keyStart = nullptr;
}