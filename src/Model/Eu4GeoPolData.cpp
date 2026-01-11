#include "Eu4GeoPolData.h"


Eu4::GeoPolData::~GeoPolData()
{
	delete mAreasData;
	delete mRegionsData;
	delete mSuperRegionsData;
	delete mContinentsData;
}

void Eu4::GeoPolData::fillColorToID(FilePathHandler*& filePathHandler) {
	FILE* file = NULL;
	auto definitionPath = filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::DEFINITION).at(0).string();
	const char* definitionPathData = definitionPath.data();
	errno_t err = fopen_s(&file, definitionPathData, "r");
	if (err != 0 || file == NULL) {
		perror("Failed to open file");
		return;
	}
	char line[256];
	uint16_t provID;
	uint8_t R;
	uint8_t G;
	uint8_t B;


	if (fgets(line, sizeof(line), file) == NULL) {
		fclose(file);
		throw std::runtime_error("Empty definition.csv file");
	}

	while (fgets(line, sizeof(line), file)) {
		int n = sscanf_s(line, "%hu;%hhu;%hhu;%hhu;", &provID, &R, &G, &B);
		if (n == 4) {
			uint32_t packedRGB = (R << 16) | (G << 8) | B;
			mProvColorToUID.insert(std::make_pair(packedRGB, provID));
			mProvUIDToColor.insert(std::make_pair(provID, packedRGB));
		}
		else {
			throw std::runtime_error("Invalid definition.csv file");
		}
	}
	fclose(file);
}

uint16_t Eu4::GeoPolData::getIDFromColor(uint32_t packedRGB) const{
	return mProvColorToUID.at(packedRGB);
}

void Eu4::GeoPolData::initData(FilePathHandler*& filePathHandler) {
	initMapInfo(filePathHandler);
	initDataProvinces(filePathHandler);
	initDataAreas(filePathHandler);
	initDataRegions(filePathHandler);
	initDataSuperRegions(filePathHandler);
	initDataContinents(filePathHandler);
}

const Eu4::Province& Eu4::GeoPolData::getProvinceData(const int& UID) const {
	return mProvinces.at(UID);
}
Eu4::Province& Eu4::GeoPolData::getProvinceData(const int& UID) {
	return mProvinces.at(UID);
}

const std::array<int, 5> Eu4::GeoPolData::getNumberPerType() const
{
	return {
		static_cast<int>(mProvinces.size()),
		static_cast<int>(mAreas.size()),
		static_cast<int>(mRegions.size()),
		static_cast<int>(mSuperRegions.size()),
		static_cast<int>(mContinents.size())
	};
}


const std::vector<Eu4::Province>& Eu4::GeoPolData::getAllProvinces() const
{
	return mProvinces;
}

const int Eu4::GeoPolData::getNbAreas() const
{
	return mAreas.size();
}

void Eu4::GeoPolData::initDataProvinces(FilePathHandler*& filePathHandler) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Province Data from file\t----\t" << std::endl;
	mProvinces.clear();
	mProvinces.resize(mMapInfo.maxProvinces + 1);

	//hack
	//auto provHistoryPath = filePathHandler->getPathsFromFolderKey(relative_path::eu4::history::PROVINCES_).at(0).parent_path().string();
	//const char* provHistoryPathPtr = provHistoryPath.data();

	std::vector<std::tuple<uint16_t, std::string, std::filesystem::path>> fileData;
	filePathHandler->populateProvincesFilePathStructure(fileData, mMapInfo.maxProvinces + 1);
	//SimpleParser::getNumberedTxtFiles(fileData, provHistoryPathPtr);




	//for (const auto& tuple : fileData)
	//{
	//	const auto& [provUID, name, path] = tuple;

	//	mProvinces[provUID].mFileData =
	//		new DM::FileData<Eu4::Province>(
	//			path.string(),
	//			filePathHandler->getExportFromFullPath(path.string()),
	//			Eu4::GeoPolData::initHelperProvince,
	//			&mProvinces[provUID],
	//			Eu4::GeoPolData::resetProvince
	//		);
	//}
	// After populateProvincesFilePathStructure
	std::unordered_map<uint16_t, int> idCount;
	for (const auto& [id, name, path] : fileData) {
		idCount[id]++;
		if (idCount[id] > 1) {
			std::cout << "DUPLICATE ID: " << id << " at path: " << path << std::endl;
		}
	}


	std::for_each(std::execution::par, fileData.begin(), fileData.end(),
		[&](const auto& tuple) {
			const auto& [provUID, name, path] = tuple;
			mProvinces[provUID].mFileData = new DM::FileData<Eu4::Province>(path.string(), 
																			filePathHandler->getExportFromFullPath(path.string()),
																			Eu4::GeoPolData::initHelperProvince,
																			&mProvinces[provUID],
																			Eu4::GeoPolData::resetProvince);
		}
	);
	std::for_each(std::execution::par, fileData.begin(), fileData.end(),
		[&](const auto& tuple) {
			const auto& [provUID, name, path] = tuple;
			mProvinces[provUID].initFromFile2(
				provUID,
				mProvUIDToColor[provUID],
				name,
				path.string()
			);
			mProvinces[provUID].mFileData->initDataTokens();
		}
	);

	fileData.clear();
	fileData.shrink_to_fit();

	for (auto id : mMapInfo.seaStarts) {
		mProvinces[id].isWater = true;
	}

	for (auto id : mMapInfo.lakes) {
		mProvinces[id].isWater = true;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " ms" << std::endl;
}

void Eu4::GeoPolData::initMapInfo(FilePathHandler*& filePathHandler)
{
	mmap::Handle handle;
	if (!mmap::open(filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::DEFAULT).at(0).string(), handle)) {
		throw std::runtime_error("Could not open file : Default Map");
	}
	const char* ptr = handle.data;
	const char* end = ptr + handle.size;

	const char* keyStart = nullptr;
	while (ptr < end) {

		switch (*ptr) {
		case '#': {
			const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
			ptr = commentEnd ? commentEnd : end;
			break;
		}
		case '{':
		case '}':
		case '\n':
		case '\t':
		case '\r':
		case ' ':
		case '=': {
			break;
		}
		default: {

			char c = *ptr;
			if (c == 'm' && *(ptr + 12) == 's') {
				ptr += 13;
				while (*ptr < '0' || *ptr > '9') {
					++ptr;
				}
				keyStart = ptr;
				while (*ptr >= '0' && *ptr <= '9') {
					++ptr;
				}
				std::from_chars(keyStart, ptr, mMapInfo.maxProvinces);
				keyStart = nullptr;
				break;
			}
			std::vector<uint16_t>* dataContainer = nullptr;
			if (c == 's' && *(ptr + 9) == 's') {
				dataContainer = &mMapInfo.seaStarts;
				ptr += 10;
			}
			else if (c == 'l' && *(ptr + 4) == 's') {
				dataContainer = &mMapInfo.lakes;
				ptr += 5;
			}
			else if (!mMapInfo.seaStarts.empty() && !mMapInfo.lakes.empty()) {
				while (ptr < end) {
					++ptr;
				}
				--ptr;
				break;
			}
			else {
				break;
			}
			while (*ptr != '{') {
				++ptr;
			}
			while (*ptr != '}') {
				while ((*ptr < '0' || *ptr > '9') && *ptr != '}') {
					if (*ptr == '#') {
						while (*ptr != '\n') ++ptr;
					}
					++ptr;
				}
				keyStart = ptr;
				while (*ptr >= '0' && *ptr <= '9') {
					++ptr;
				}
				uint16_t value;
				std::from_chars(keyStart, ptr, value);
				dataContainer->push_back(value);
			}


			keyStart = nullptr;
		}
		}
		++ptr;
	}
	mmap::close(handle);
}

void Eu4::GeoPolData::initDataAreas(FilePathHandler*& filePathHandler) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Area Data from file\t----\t";

	const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::AREA).at(0).string();
	mAreasData = new DM::FileData<Eu4::GeoPolData>(path, filePathHandler->getExportFromFullPath(path));
	mAreasData->initData(Eu4::GeoPolData::initHelperArea, this, Eu4::GeoPolData::resetAreas);

	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;
}

void Eu4::GeoPolData::initDataRegions(FilePathHandler*& filePathHandler)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Region Data from file\t----\t";

	const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::REGION).at(0).string();
	mRegionsData = new DM::FileData<Eu4::GeoPolData>(path, filePathHandler->getExportFromFullPath(path));
	mRegionsData->initData(Eu4::GeoPolData::initHelperRegion, this, Eu4::GeoPolData::resetRegions);


	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;
}


void Eu4::GeoPolData::initDataSuperRegions(FilePathHandler*& filePathHandler) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate SuperRegion Data from file\t----\t";

	const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::SUPERREGION).at(0).string();
	mSuperRegionsData = new DM::FileData<Eu4::GeoPolData>(path, filePathHandler->getExportFromFullPath(path));
	mSuperRegionsData->initData(Eu4::GeoPolData::initHelperSuperRegion, this, Eu4::GeoPolData::resetSuperRegions);

	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;

}


void Eu4::GeoPolData::initDataContinents(FilePathHandler*& filePathHandler) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Continent Data from file\t----\t";


	const std::string& path = filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::CONTINENT).at(0).string();
	mContinentsData = new DM::FileData<Eu4::GeoPolData>(path, filePathHandler->getExportFromFullPath(path));
	mContinentsData->initData(Eu4::GeoPolData::initHelperContinent, this, Eu4::GeoPolData::resetContinents);

	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;
}




void Eu4::GeoPolData::initHelperProvince(DM::FileData<Eu4::Province>& fileData, Eu4::Province& prov)
{
	const char* ptr = prov.mFileData->mBuffer.data();
	const char* end = ptr + prov.mFileData->mBuffer.size();
	std::vector<DM::DataToken>& dataTokens = prov.mFileData->mDataTokens;
	int iteration = 0;
	const int MAX_ITERATIONS = 100000; // Safety limit
	while (ptr < end) {
		if (++iteration > MAX_ITERATIONS) {
			std::cerr << "RUNAWAY DETECTED in province parsing!\n";
			std::cerr << "Buffer size: " << prov.mFileData->mBuffer.size() << "\n";
			std::cerr << "Current position: " << (ptr - prov.mFileData->mBuffer.data()) << "\n";
			std::cerr << "Remaining: " << (end - ptr) << "\n";
			if (end - ptr > 0) {
				size_t count = (end - ptr) > 100 ? 100 : (end - ptr);
				std::cerr << "Last 100 chars:\n" << std::string(ptr, count) << "\n";
			}
			break;
		}

		if (ptr >= end) {
			std::cerr << "WARNING: ptr exceeded end!\n";
			break;
		}
		switch (*ptr) {
		case '\n':
		case '\t':
		case '\r':
		case ' ':
		case '=': {
			break;
		}
		case '#': {
			const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
			ptr = commentEnd ? commentEnd : end;
			break;
		}

		default: {
			uint16_t* value = nullptr;
			switch (*ptr) {
			case 'a': {
				//add_core
				if (*(ptr + 7) == 'e') {
					ptr += 8;
					value = &(prov.mCoresID.emplace_back(0));
				}
				//add_local_autonomy
				else if (*(ptr + 17) == 'y') {
					ptr += 18;
					value = &(prov.mAddAutonomy);
				}
				// add_province_triggered_modifier
				// TODO probable bugs and capture the actual data
				else if (*(ptr + 12) == '_' && *(ptr + 30) == 'r') {
					ptr += 31;
					value = &(prov.mTrigMod.emplace_back(0));
				}

				break;
			}
			case 'b': {
				// base_tax
				if (*(ptr + 7) == 'x') {
					ptr += 8;
					value = &(prov.mBaseTax);
				}
				// base_production
				else if (*(ptr + 14) == 'n') {
					ptr += 15;
					value = &(prov.mBaseProduction);
				}
				// base_manpower
				else if (*(ptr + 12) == 'r') {
					ptr += 13;
					value = &(prov.mBaseManpower);
				}
				break;
			}
			case 'c': {
				/* controller */
				if (*(ptr + 9) == 'r') {
					ptr += 10;
					value = &(prov.mControllerID);
				}
				/* capital */
				else if (*(ptr + 6) == 'l') {
					ptr += 7;
					Eu4::GeoPolData::parserCaptureCapital(ptr, end, dataTokens, prov.mCapital);
					goto skip_to;
				}
				/* culture */
				else if (*(ptr + 6) == 'e') {
					ptr += 7;
					value = &(prov.mCultureID);
				}
				/* center_of_trade */
				else if (*(ptr + 14) == 'e') {
					ptr += 15;
					value = &(prov.mCenterofTrade);
				}
				break;
			}
			case 'd': {
				// discovered_by
				if (*(ptr + 12) == 'y') {
					ptr += 13;
					value = &(prov.mDiscoveredBy.emplace_back(0));
				}
				break;
			}
			case 'e': {
				// extra_cost
				if (*(ptr + 9) == 't') {
					ptr += 10;
					value = &(prov.mExtraCost);
				}
				break;
			}
			case 'f': {
				// fort_15th
				if (*(ptr + 8) == 'h') {
					ptr += 9;
					value = &(prov.mFort);
				}
				break;
			}
			case 'h': {
				// hre
				if (*(ptr + 2) == 'e') {
					ptr += 3;
					value = &(prov.mIsHre);
				}
				break;
			}
			case 'i': {
				/* is_city */
				if (*(ptr + 6) == 'y') {
					ptr += 7;
					value = &(prov.mIsCity);
				}
				break;
			}
			case 'l': {
				// latent_trade_goods
				if (*(ptr + 17) == 's') {
					ptr += 18;
					Eu4::GeoPolData::parserCaptureAllValuesBracket(ptr, dataTokens, prov.mLatentTradeGood);
					goto skip_to;
					break;
				}
			}
			case 'n': {
				// native_size
				if (*(ptr + 10) == 'e') {
					ptr += 11;
					value = &(prov.mNativeSize);
				}
				// native_ferocity
				else if (*(ptr + 14) == 'y') {
					ptr += 15;
					value = &(prov.mNativeFerocity);
				}
				// native_hostileness
				else if (*(ptr + 17) == 's') {
					ptr += 18;
					value = &(prov.mNativeHostile);
				}

				break;
			}
			case 'o': {
				/* owner */
				if (*(ptr + 4) == 'r') {
					ptr += 5;
					value = &(prov.mOwnerID);
				}
				break;
			}
			case 'r': {
				/* religion */
				if (*(ptr + 7) == 'n') {
					ptr += 8;
					value = &(prov.mReligionID2);
				}
				break;
			}
			case 't': {
				// trade_goods
				if (*(ptr + 10) == 's') {
					ptr += 11;
					value = &(prov.mTradeGood);
				}
				// tribal_owner
				else if (*(ptr + 11) == 'r') {
					ptr += 12;
					value = &(prov.mTribalOwner);
				}
				break;
			}
			default: {
			}
			}
			if (value != nullptr) {
				Eu4::GeoPolData::parserSkipUntilValueStd(ptr, dataTokens);
				*value = prov.mFileData->mDataTokens.size() - 1;
			}
			else {
				ptr = end;
			}
		}
		}
	skip_to:
		++ptr;
	}

}

void Eu4::GeoPolData::initHelperArea(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData)
{

	const char* ptr = fileData.mBuffer.data();
	const char* end = ptr + fileData.mBuffer.size();

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
			while (*ptr != '}') {
				switch (*ptr) {
				case '\n':
				case '\t':
				case ' ':
				case '\r':
					break;
				case '#': {
					while (ptr < end && *ptr != '\n' && *ptr != '\r') {
						++ptr;
					}
					break;
				}
				case 'c': {
					while (*ptr != '}') {
						++ptr;
					}
					++ptr;
					break;
				}
				default: {
					keyStart = ptr;
					while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '\r') {
						++ptr;
					}

					uint16_t value;
					std::from_chars(keyStart, ptr, value);
					GeoPolData.mAreas.back().mGeoPolIDs.push_back(value);
					keyStart = nullptr;
					--ptr;
				}
				}

				++ptr;
			}
			break;
		}
		case '\n':
		case '\t':
		case ' ':
		case '\r':
		case '=': {
			break;
		}
		default: {
			keyStart = ptr;
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=' && *ptr != '\r') {
				++ptr;
			}

			GeoPolData.mAreas.emplace_back(Eu4::Area());
			Eu4::Area& area = GeoPolData.mAreas.back();
			fileData.mDataTokens.emplace_back(DM::DataToken());
			fileData.mDataTokens.back().mPtrStart = keyStart;
			fileData.mDataTokens.back().mLength = ptr - keyStart;
			area.mNameID = fileData.mDataTokens.size() - 1;

			GeoPolData.mAreasNameToArea.emplace(fileData.mDataTokens.back().getOriginName(), GeoPolData.mAreas.size() - 1);
			keyStart = nullptr;
		}
		}
		++ptr;
	}

	for (int i = 0; i < GeoPolData.mAreas.size(); ++i) {
		for (const uint16_t UID : GeoPolData.mAreas[i].mGeoPolIDs) {
			GeoPolData.mProvinces[UID].mAreaID = i;
		}
	}
}

void Eu4::GeoPolData::initHelperRegion(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData)
{
	const char* ptr = fileData.mBuffer.data();
	const char* end = ptr + fileData.mBuffer.size();

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
			while (*ptr != '}') {
				switch (*ptr) {
				case'#': {
					const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
					ptr = commentEnd ? commentEnd : end;
				}
				case '\n':
				case '\t':
				case '\r':
				case ' ':
					break;
				default: {
					if (*(ptr + 4) == 's') {
						ptr += 5;
						ptr = (const char*)memchr(ptr, '{', end - ptr);
						++ptr;

						while (*ptr != '}') {
							while ((*ptr < 97 || *ptr > 122) && *ptr != '}' && *ptr != '#') {
								++ptr;
							}
							if (*ptr == '}') {
								continue;
							}
							if (*ptr == '#') {
								ptr = (const char*)memchr(ptr, '\n', end - ptr);
								continue;
							}
							keyStart = ptr;
							while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '\r') {
								++ptr;
							}
							Eu4::Region& region = GeoPolData.mRegions.back();
							fileData.mDataTokens.emplace_back(DM::DataToken());
							fileData.mDataTokens.back().mPtrStart = keyStart;
							fileData.mDataTokens.back().mLength = ptr - keyStart;
							region.data2.emplace_back(fileData.mDataTokens.size() - 1);
							keyStart = nullptr;
						}
					}
					else if (*(ptr + 6) == 'n') {
						//monsoon isnt taken into account for now but it would be ther
						/*while (*ptr != '}') {
							++ptr;
						}*/
						ptr = (const char*)memchr(ptr, '}', end - ptr);

					}
				}
				}

				++ptr;
			}
			break;
		}
		case '\n':
		case '\t':
		case ' ':
		case '\r':
		case '=': {
			break;
		}
		default: {
			keyStart = ptr;
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=' && *ptr != '\r') {
				++ptr;
			}

			GeoPolData.mRegions.emplace_back(Eu4::Region());
			Eu4::Region& region = GeoPolData.mRegions.back();
			fileData.mDataTokens.emplace_back(DM::DataToken());
			fileData.mDataTokens.back().mPtrStart = keyStart;
			fileData.mDataTokens.back().mLength = ptr - keyStart;
			region.mNameID = fileData.mDataTokens.size() - 1;
			
			GeoPolData.mRegionNameToIndex.emplace(fileData.mDataTokens.back().getOriginName(), GeoPolData.mRegions.size() - 1);
			keyStart = nullptr;
		}
		}
		++ptr;
	}

	for (int i = 0; i < GeoPolData.mRegions.size(); ++i) {
		for (const auto& index : GeoPolData.mRegions[i].data2) {
			for (const uint16_t UID : GeoPolData.mAreas[GeoPolData.mAreasNameToArea.at(fileData.mDataTokens[index].getOriginName())].mGeoPolIDs) {
				GeoPolData.mProvinces[UID].mRegionID = i;
			}
		}
	}
}

void Eu4::GeoPolData::initHelperSuperRegion(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData)
{
	const char* ptr = fileData.mBuffer.data();
	const char* end = ptr + fileData.mBuffer.size();

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
			while (*ptr != '}') {
				switch (*ptr) {
					case'#': {
						const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
						ptr = commentEnd ? commentEnd : end;
					}
					case '\n':
					case '\t':
					case '\r':
					case ' ':
						break;
					default:
						keyStart = ptr;
						while (*ptr != ' ' && *ptr != '\n' && *ptr != '\t' && *ptr != '#' && *ptr != '\r') {
							++ptr;
						}
						if (std::string(keyStart, ptr) != "restrict_charter") {
							Eu4::SuperRegion& superRegion = GeoPolData.mSuperRegions.back();
							fileData.mDataTokens.emplace_back(DM::DataToken());
							fileData.mDataTokens.back().mPtrStart = keyStart;
							fileData.mDataTokens.back().mLength = ptr - keyStart;
							superRegion.data2.emplace_back(fileData.mDataTokens.size() - 1);
						}
						keyStart = nullptr;
				}
				++ptr;
			}
			break;
		}
		case '\n':
		case '\t':
		case ' ':
		case '\r':
		case '=': {
			break;
		}
		default: {
			keyStart = ptr;
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=' && *ptr != '\r') {
				++ptr;
			}
			GeoPolData.mSuperRegions.emplace_back(Eu4::SuperRegion());
			Eu4::SuperRegion& superRegion = GeoPolData.mSuperRegions.back();
			superRegion.mName = std::string(keyStart, ptr);
			fileData.mDataTokens.emplace_back(DM::DataToken());
			fileData.mDataTokens.back().mPtrStart = keyStart;
			fileData.mDataTokens.back().mLength = ptr - keyStart;
			superRegion.mNameID = fileData.mDataTokens.size() - 1;
			
			GeoPolData.mSuperRegionNameToIndex.emplace(fileData.mDataTokens.back().getOriginName(), GeoPolData.mSuperRegions.size() - 1);
			keyStart = nullptr;
		}
		}
		++ptr;
	}


	for (int y = 0; y < GeoPolData.mSuperRegions.size(); ++y) {
		for (uint16_t index : GeoPolData.mSuperRegions[y].data2) {

			auto regionIndex = fileData.mDataTokens[index].getOriginName();
			Eu4::Region& region = GeoPolData.mRegions[GeoPolData.mRegionNameToIndex.at(regionIndex)];
			
			//DM::FileData* regionDataFile = GeoPolData.fileManager->mFiles.at(GeoPolData.mRegionDataID);

			for (auto& areaTokenIndex : region.data2) {
				auto& areaToken = GeoPolData.mRegionsData->mDataTokens.at(areaTokenIndex);
				auto areaName = areaToken.getOriginName();
				
				for (const uint16_t UID : GeoPolData.mAreas[GeoPolData.mAreasNameToArea[areaName]].mGeoPolIDs) {
					GeoPolData.mProvinces[UID].mSuperRegionID = y;
				}
				
			}
			
		}
	}


}

void Eu4::GeoPolData::initHelperContinent(DM::FileData<Eu4::GeoPolData>& fileData, Eu4::GeoPolData& GeoPolData)
{
	const char* ptr = fileData.mBuffer.data();
	const char* end = ptr + fileData.mBuffer.size();

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
			while (*ptr != '}') {
				switch (*ptr) {
				case'#': {
					const char* commentEnd = (const char*)memchr(ptr, '\n', end - ptr);
					ptr = commentEnd ? commentEnd : end;
				}
				case '\n':
				case '\t':
				case '\r':
				case ' ':
					break;
				default:
					keyStart = ptr;
					ptr += strcspn(ptr, "\n \t#=\r");
					Eu4::Continent& continent = GeoPolData.mContinents.back();
					uint16_t value;
					std::from_chars(keyStart,ptr,value);
					continent.mGeoPolIDs.push_back(value);
					keyStart = nullptr;
				}
				++ptr;
			}
			break;
		}
		case '\n':
		case '\t':
		case '\r':
		case ' ':
		case '=': {
			break;
		}
		default: {
			keyStart = ptr;
			ptr += strcspn(ptr, "\n \t#=\r");
			if (std::string(keyStart, ptr) != "island_check_provinces") {
				GeoPolData.mContinents.emplace_back(Eu4::Continent());
				Eu4::Continent& continent = GeoPolData.mContinents.back();

				continent.mName = std::string(keyStart, ptr);
				continent.testName.mPtrStart = keyStart;
				continent.testName.mLength = ptr - keyStart;

				GeoPolData.mContinentNameToIndex.emplace(continent.testName.getOriginName(), GeoPolData.mContinents.size() - 1);
			}
			else {
				ptr += strcspn(ptr, "}");
			}
			keyStart = nullptr;
		}
		}
		++ptr;
	}

	for (int i = 0; i < GeoPolData.mContinents.size(); ++i) {
		for (auto& id : GeoPolData.mContinents[i].mGeoPolIDs) {
			GeoPolData.mProvinces[id].mContinentID = i;
		}
	}


}

void Eu4::GeoPolData::resetProvince(Eu4::Province& prov)
{
	prov.resetData();
}

void Eu4::GeoPolData::resetAreas(Eu4::GeoPolData& data)
{
}

void Eu4::GeoPolData::resetRegions(Eu4::GeoPolData& data)
{
}

void Eu4::GeoPolData::resetSuperRegions(Eu4::GeoPolData& data)
{
}

void Eu4::GeoPolData::resetContinents(Eu4::GeoPolData& data)
{
}

inline void Eu4::GeoPolData::parserSkipUntilValueStd(const char*& ptr, std::vector<DM::DataToken>& dataTokens)
{
	while (*ptr != '=') ++ptr;
	++ptr;
	while (*ptr == ' ' || *ptr == '\t'  || *ptr == '\"') ++ptr;
	const char* keyStart = ptr;
	while (*ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && *ptr != '#' && *ptr != '\"') ++ptr;
	dataTokens.emplace_back(DM::DataToken());
	dataTokens.back().mPtrStart = keyStart;
	dataTokens.back().mLength = ptr - keyStart;
	keyStart = nullptr;
}

inline void Eu4::GeoPolData::parserCaptureAllValuesBracket(const char*& ptr, std::vector<DM::DataToken>& dataTokens, std::vector<uint16_t>& container)
{
	while (*ptr != '=') ++ptr;
	++ptr;
	while (*ptr != '{') ++ptr;
	++ptr;
	int safety = 0;
	while (*ptr != '}') {
		if (++safety > 100) {
			std::cerr << "INFINITE LOOP in parserCaptureAllValuesBracket!\n";
			std::cerr << "Current char: '" << *ptr << "' (ASCII " << (int)*ptr << ")\n";
			std::cerr << "Last 50 chars: " << std::string(ptr - 50, 50) << "\n";
			break;
		}
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') ++ptr;
		const char* keyStart = ptr;
		while (*ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && *ptr != '#' && *ptr != '\"') ++ptr;
		if (*ptr == '#') {
			while (*ptr != '\n' && *ptr != '\r') ++ptr;
		}
		else {
			dataTokens.emplace_back(DM::DataToken());
			dataTokens.back().mPtrStart = keyStart;
			dataTokens.back().mLength = ptr - keyStart;
			container.emplace_back(dataTokens.size() - 1);
			keyStart = nullptr;
		}
		++ptr;
	}
}

inline void Eu4::GeoPolData::parserCaptureCapital(const char*& ptr, const char* end, std::vector<DM::DataToken>& dataTokens, uint16_t& capital)
{
	std::cout << "  parserCaptureCapital start, remaining: " << (end - ptr) << "\n";

	while (ptr < end && *ptr != '=') ++ptr;
	if (ptr >= end) { std::cerr << "  No = found\n"; return; }
	++ptr;

	while (ptr < end && *ptr != '\"') ++ptr;
	if (ptr >= end) { std::cerr << "  No opening quote\n"; return; }
	++ptr;

	const char* keyStart = ptr;

	while (ptr < end && *ptr != '\"') ++ptr;
	if (ptr >= end) { std::cerr << "  No closing quote\n"; return; }

	dataTokens.emplace_back(DM::DataToken());
	dataTokens.back().mPtrStart = keyStart;
	dataTokens.back().mLength = ptr - keyStart;
	capital = dataTokens.size() - 1;

	std::cout << "  parserCaptureCapital end, captured length: " << (ptr - keyStart) << "\n";
}

