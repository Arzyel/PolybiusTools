#include "Data.h"

Eu4::Data::Data(FilePathHandler*& filePathHandler)
{
	mCultRelData.initData(filePathHandler);
	mCountryData.initializeData();
	mTGData.initData(filePathHandler);
	mGeoPolData.fillColorToID();
	mGeoPolData.initData(filePathHandler);
}
