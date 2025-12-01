#include "Data.h"

Eu4::Data::Data(FilePathHandler*& filePathHandler)
{
	std::vector<fs::path> ket1 = filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::CULTURES_);
	mCultRelData.loadCultureData(ket1);
	mCultRelData.loadReligionData(R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\religions\00_religion.txt)");
	//mCultRelData.loadReligionData(filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::CULTURES_));

	mCountryData.initializeData();

	mGeoPolData.fillColorToID();
	mGeoPolData.initData(filePathHandler);

}
