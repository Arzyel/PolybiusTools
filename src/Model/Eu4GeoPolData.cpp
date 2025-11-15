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

void Eu4::GeoPolData::initData(FilePathHandler*& filePathHandler) {
	initDataProvinces();
	initDataAreas(filePathHandler);
	initDataRegions();
	initDataSuperRegions();
	initDataContinents();
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
	mmap::Handle handle;
	if (!mmap::open(test, handle)) {
		throw std::runtime_error("Could not open file : " + test);
	}

	const char* ptr = handle.data;
	const char* end = ptr + handle.size;

	const char* keyStart = nullptr;
	const char* keyEnd = nullptr;

	const char* valueStart = nullptr;
	const char* valueEnd = nullptr;

	while (ptr < end) {
		char c = *ptr;
		switch (c) {
		case '#': {
			while (ptr < end && *ptr != '\n') {
				++ptr;
			}
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
						while (ptr < end && *ptr != '\n') {
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
						while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#') {
							if (valueStart == nullptr) {
								valueStart = ptr;
							}
							++ptr;

						}
						uint16_t value;
						std::from_chars(valueStart, ptr, value);
						mAreas.back().mGeoPolIDs.push_back(value);
						valueStart = nullptr;
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
		case '=': {
			break;
		}
		default: {
			while (*ptr != '\n' && *ptr != ' ' && *ptr != '\t' && *ptr != '#' && *ptr != '=') {
				if (keyStart == nullptr) {
					keyStart = ptr;
				}
				++ptr;
			}

			mAreas.emplace_back(Area());
			mAreas.back().mName = std::string(keyStart, ptr);
			keyStart = nullptr;
		}
		}
		++ptr;
	}

	mmap::close(handle);



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
void Eu4::GeoPolData::initDataRegions() {

}
void Eu4::GeoPolData::initDataSuperRegions() {

}
void Eu4::GeoPolData::initDataContinents() {

}
