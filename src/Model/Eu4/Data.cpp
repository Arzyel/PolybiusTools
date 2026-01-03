#include "Data.h"

Eu4::Data::Data(FilePathHandler*& filePathHandler)
{
	mCultRelData.initData(filePathHandler);
	mCountryData.initializeData();
	mGeoPolData.fillColorToID();
	mGeoPolData.initData(filePathHandler);
	mTGData.initData(filePathHandler);
}
