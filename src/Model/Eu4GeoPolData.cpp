#include "Eu4GeoPolData.h"

void Eu4GeoPolData::fillColorToID()
{
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
		printf("File is empty\n");
		fclose(file);
		return;
	}


	while (fgets(line, sizeof(line), file)) {
		int n = sscanf_s(line, "%hu;%hhu;%hhu;%hhu;", &provID, &R, &G, &B);
		if (n == 4) {
			uint32_t packedRGB = (R << 16) | (G << 8) | B;
			locColorToID.insert(std::make_pair(packedRGB, provID));
			locIDToColor.insert(std::make_pair(provID, packedRGB));
		}
		else {
			throw std::runtime_error("Invalid definition.csv file");
		}
	}

	fclose(file);

}

uint16_t Eu4GeoPolData::getIDFromColor(uint32_t packedRGB) const
{
	return locColorToID.at(packedRGB);
}

void Eu4GeoPolData::initLocationData()
{
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << "Initiate Location Data from file\t----\t";
	mLocations.clear();
	mLocations.resize(locColorToID.size() + 500);

	// No copy - reuse vector
	std::vector<std::tuple<uint16_t, std::string, std::filesystem::path>> fileData;
	SimpleParser::getNumberedTxtFiles(fileData, PROV_HISTORY_FOLDER);

	std::for_each(std::execution::par, fileData.begin(), fileData.end(),
		[&](const auto& tuple) {
			const auto& [provUID, name, path] = tuple;
			mLocations.at(provUID).initFromFile(
				std::to_string(provUID),
				locIDToColor.at(provUID),
				path.string(),
				name
			);
		}
	);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Elapsed Time : " << elapsed.count() << " ms" << std::endl;
}





