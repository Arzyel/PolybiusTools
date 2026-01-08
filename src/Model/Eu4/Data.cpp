#include "Data.h"

Eu4::Data::Data(FilePathHandler*& filePathHandler)
{
	mCultRelData.initData(filePathHandler);
	mCountryData.initializeData(filePathHandler);
	mTGData.initData(filePathHandler);
	mGeoPolData.fillColorToID(filePathHandler);
	mGeoPolData.initData(filePathHandler);
}
