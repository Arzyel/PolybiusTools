#include "Eu4GeoPolData.h"

//void Eu4GeoPolData::fillColorToID()
//{
//	FILE* file = NULL;
//		errno_t err = fopen_s(&file, DEFINITIONS, "r");
//		if (err != 0 || file == NULL) {
//			perror("Failed to open file");
//			return;
//		}
//	char line[256];
//	uint16_t provID;
//	uint8_t R;
//	uint8_t G;
//	uint8_t B;
//
//
//	if (fgets(line, sizeof(line), file) == NULL) {
//		printf("File is empty\n");
//		fclose(file);
//		return;
//	}
//
//
//	while (fgets(line, sizeof(line), file)) {
//		int n = sscanf_s(line, "%hu;%hhu;%hhu;%hhu;", &provID, &R, &G, &B);
//		if (n == 4) {
//			uint32_t packedRGB = (R << 16) | (G << 8) | B;
//			locColorToID.insert(std::make_pair(packedRGB, provID));
//			locIDToColor.insert(std::make_pair(provID, packedRGB));
//		}
//		else {
//			throw std::runtime_error("Invalid definition.csv file");
//		}
//	}
//
//	fclose(file);
//
//}
//
//uint16_t Eu4GeoPolData::getIDFromColor(uint32_t packedRGB) const
//{
//	return locColorToID.at(packedRGB);
//}
//
//void Eu4GeoPolData::initLocationData()
//{
//	auto start = std::chrono::high_resolution_clock::now();
//	std::cout << "Initiate Location Data from file\t----\t";
//	mLocations.clear();
//	mLocations.resize(locColorToID.size() + 500);
//
//	// No copy - reuse vector
//	std::vector<std::tuple<uint16_t, std::string, std::filesystem::path>> fileData;
//	SimpleParser::getNumberedTxtFiles(fileData, PROV_HISTORY_FOLDER);
//
//	std::for_each(std::execution::par, fileData.begin(), fileData.end(),
//		[&](const auto& tuple) {
//			const auto& [provUID, name, path] = tuple;
//			mLocations.at(provUID).initFromFile(
//				std::to_string(provUID),
//				locIDToColor.at(provUID),
//				path.string(),
//				name
//			);
//		}
//	);
//
//	auto end = std::chrono::high_resolution_clock::now();
//	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//	std::cout << "Elapsed Time : " << elapsed.count() << " ms" << std::endl;
//}
//




///---------------------TEST new version -----------------------------//////////

void Eu4::GeoPolData::fillColorToID() {
	FILE* file = NULL;
	errno_t err = fopen_s(&file, DEFINITIONS, "r");
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

void Eu4::GeoPolData::initData(FilePathHandler*& filePathHandler, DM::FileManager* fileManager) {
	this->fileManager = fileManager;
	initDataProvinces();
	initDataAreas(filePathHandler);
	initDataRegions(filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::REGION).at(0).string());
	initDataSuperRegions(filePathHandler);
	//initDataContinents(filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::CONTINENT).at(0).string());
}

const Eu4::Province& Eu4::GeoPolData::getProvinceData(const int& UID) const {
	return mProvinces.at(UID);
}

void Eu4::GeoPolData::initDataProvinces() {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Province Data from file\t----\t";
	mProvinces.clear();
	mProvinces.resize(mProvUIDToColor.size() + 1);

	// No copy - reuse vector
	std::vector<std::tuple<uint16_t, std::string, std::filesystem::path>> fileData;
	SimpleParser::getNumberedTxtFiles(fileData, PROV_HISTORY_FOLDER);

	std::for_each(std::execution::par, fileData.begin(), fileData.end(),
		[&](const auto& tuple) {
			const auto& [provUID, name, path] = tuple;
			mProvinces.at(provUID).initFromFile(
				std::to_string(provUID),
				mProvUIDToColor.at(provUID),
				path.string(),
				name
			);
		}
	);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " ms" << std::endl;
}
void Eu4::GeoPolData::initDataAreas(FilePathHandler*& filePathHandler) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Area Data from file\t----\t";
	auto test = filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::AREA).at(0).string();

	Eu4::GeoPolData::helperReadData<Eu4::Area>(test, mAreas, mAreasNameToArea);
	//mmap::Handle handle;
	//if (!mmap::open(test, handle)) {
	//	throw std::runtime_error("Could not open file : " + test);
	//}

	//const char* ptr = handle.data;
	//const char* end = ptr + handle.size;

	//const char* keyStart = nullptr;
	//const char* keyEnd = nullptr;

	//const char* valueStart = nullptr;
	//const char* valueEnd = nullptr;

	//while (ptr < end) {
	//	char c = *ptr;
	//	switch (c) {
	//	case '#': {
	//		while (ptr < end && *ptr != '\n') {
	//			++ptr;
	//		}
	//		break;
	//	}
	//	case '{': {
	//		++ptr;
	//		while (*ptr != '}') {
	//			switch (*ptr) {
	//				case '\n':
	//				case '\t':
	//				case ' ':
	//					break;
	//				case '#': {
	//					while (ptr < end && *ptr != '\n') {
	//						++ptr;
	//					}
	//					break;
	//				}
	//				case 'c': {
	//					while (*ptr != '}') {
	//						++ptr;
	//					}
	//					++ptr;
	//					break;
	//				}
	//				default: {
	//					while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#') {
	//						if (valueStart == nullptr) {
	//							valueStart = ptr;
	//						}
	//						++ptr;

	//					}
	//					uint16_t value;
	//					std::from_chars(valueStart, ptr, value);
	//					mAreas.back().mGeoPolIDs.push_back(value);
	//					valueStart = nullptr;
	//					--ptr;
	//				}
	//			}

	//			++ptr;
	//		}
	//		break;
	//	}
	//	case '\n':
	//	case '\t':
	//	case ' ':
	//	case '=': {
	//		break;
	//	}
	//	default: {
	//		while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
	//			if (keyStart == nullptr) {
	//				keyStart = ptr;
	//			}
	//			++ptr;
	//		}

	//		mAreas.emplace_back(Area());
	//		mAreas.back().mName = std::string(keyStart, ptr);
	//		keyStart = nullptr;
	//	}
	//	}
	//	++ptr;
	//}
	//mmap::close(handle);



	// Add area id to corresponding province
	for (int i = 0; i < mAreas.size(); ++i) {
		for (const uint16_t UID : mAreas[i].mGeoPolIDs) {
			mProvinces.at(UID).mAreaID = i;
		}
	}


	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;
}
void Eu4::GeoPolData::initDataRegions(const std::string& filePath) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Region Data from file\t----\t";

	mmap::Handle handle;
	if (!mmap::open(filePath, handle)) {
		throw std::runtime_error("Could not open file : " + filePath);
	}

	const char* ptr = handle.data;
	const char* end = ptr + handle.size;

	const char* keyStart = nullptr;
	const char* valueStart = nullptr;

	while (ptr < end) {

		switch (*ptr) {
		case '#': {
			/*while (ptr < end && *ptr != '\n') {
				++ptr;
			}
			break;*/
			// Skip comment until '\n' efficiently
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
					break;
				case '#': {
					/*while (ptr < end && *ptr != '\n') {
						++ptr;
					}*/
					ptr = (const char*)memchr(ptr, '\n', end - ptr);
					break;
				}

				default: {
					if (*(ptr + 4) == 's') {
						ptr += 5;
						/*while (*ptr != '{') {
							++ptr;
						}*/
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
								/*while (*ptr != '\n') {
									++ptr;
								}*/
								ptr = (const char*)memchr(ptr, '\n', end - ptr);
								continue;
							}
							valueStart = ptr;
							while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#') {
								++ptr;
							}
							mRegions.back().mGeoPolContainedNames.push_back(std::string(valueStart, ptr));
							valueStart = nullptr;
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
		case '=': {
			break;
		}
		default: {
			keyStart = ptr;
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
				++ptr;
			}

			mRegions.emplace_back(Eu4::Region());
			mRegions.back().mName = std::string(keyStart, ptr);
			//mRegionNameToIndex.insert(std::make_pair(mRegions.back().mName, mRegions.size() - 1));
			mRegionNameToIndex.emplace(mRegions.back().mName, mRegions.size() - 1);
			keyStart = nullptr;
		}
		}
		++ptr;
	}

	mmap::close(handle);

	// Add region id to corresponding province
	/*#pragma omp parallel for*/
	for (int i = 0; i < mRegions.size(); ++i) {
		for (const auto& area : mRegions[i].mGeoPolContainedNames) {
			for (const uint16_t UID : mAreas[mAreasNameToArea.at(area)].mGeoPolIDs) {
				mProvinces.at(UID).mRegionID = i;
			}
		}
	}


	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;
}
void Eu4::GeoPolData::initDataSuperRegions(FilePathHandler*& filePathHandler) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate SuperRegion Data from file\t----\t";


	fileManager->mFiles.push_back(DM::FileData());
	mSuperRegionDataID = fileManager->mFiles.size() - 1;
	fileManager->mFiles.back().initData<Eu4::GeoPolData>(filePathHandler->getPathsFromFolderKey(relative_path::eu4::map::SUPERREGION).at(0).string(),
		filePathHandler,
		 initHelperSuperRegion,
		*this);




	auto time_end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " us" << std::endl;


}
void Eu4::GeoPolData::initDataContinents(const std::string& filePath) {

}

void Eu4::GeoPolData::initHelperSuperRegion(DM::FileData& fileData, Eu4::GeoPolData& GeoPolData)
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
					case ' ':
						break;
					default:
						keyStart = ptr;
						while (*ptr != ' ' && *ptr != '\n' && *ptr != '\t' && *ptr != '#') {
							++ptr;
						}
						if (std::string(keyStart, ptr) != "restrict_charter") {
							Eu4::SuperRegion& superRegion = GeoPolData.mSuperRegions.back();
							superRegion.data.emplace_back(DM::DataToken());
							superRegion.data.back().mPtrStart = keyStart;
							superRegion.data.back().mLength = ptr - keyStart;
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
		case '=': {
			break;
		}
		default: {
			keyStart = ptr;
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
				++ptr;
			}

			GeoPolData.mSuperRegions.emplace_back(Eu4::SuperRegion());
			Eu4::SuperRegion& superRegion = GeoPolData.mSuperRegions.back();
			
			superRegion.mName = std::string(keyStart, ptr);
			superRegion.testName.mPtrStart = keyStart;
			superRegion.testName.mLength = ptr - keyStart;

			

			GeoPolData.mSuperRegionNameToIndex.emplace(superRegion.testName.getOriginName(), GeoPolData.mSuperRegions.size() - 1);
			keyStart = nullptr;
		}
		}
		++ptr;
	}

	for (int y = 0; y < GeoPolData.mSuperRegions.size(); ++y) {

		for (DM::DataToken& region : GeoPolData.mSuperRegions[y].data) {
			for (const auto& area : GeoPolData.mRegions[GeoPolData.mRegionNameToIndex[region.getOriginName()]].mGeoPolContainedNames) {
				for (const uint16_t UID : GeoPolData.mAreas[GeoPolData.mAreasNameToArea[area]].mGeoPolIDs) {
					GeoPolData.mProvinces.at(UID).mSuperRegionID = y;
				}
			}
		}
	}


}

void Eu4::GeoPolData::initHelperContinent(DM::FileData& fileData, Eu4::GeoPolData& GeoPolData)
{

}
