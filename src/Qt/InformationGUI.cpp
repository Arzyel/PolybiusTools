#include "InformationGUI.h"




InformationGUI::InformationGUI(const Eu4::GeoPolData& refGeoPolCont, const CultRelContainer&  refCultRel, const CountryContainer& countryContainer, QWidget* parent)
	:QTabWidget(parent), mRefGeoPolCont(refGeoPolCont), mRefCultRelCont(refCultRel), mRefCountryContainer(countryContainer)
{

}

void InformationGUI::loadWidgets()
{
	// Province tab with layout and optional group box
	LocMainWidget* provinceTab = new LocMainWidget();
	this->addTab(provinceTab, "Province");

	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab, &LocMainWidget::loadProvInfo);

	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab->findChild<DevBox*>(), &DevBox::loadDevInfo);
	
	QObject::connect(this, &InformationGUI::signalInitCultRelInfo,
		provinceTab->findChild<CultRelBox*>(), &CultRelBox::initializeData);
	
	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab->findChild<CultRelBox*>(), &CultRelBox::loadProvInfo);


	QObject::connect(this, &InformationGUI::signalInitOwnership,
		provinceTab->findChild<OwnershipBox*>(), &OwnershipBox::initializeData);
	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab->findChild<OwnershipBox*>(), &OwnershipBox::loadProvInfo);


	auto updater = [this](uint16_t Eu4::Province::* memberPtr, const std::string& newData) {
		updateProvinceField(memberPtr, newData);
		};

	provinceTab->findChild<CultRelBox*>()->makeConnections(updater);
	provinceTab->findChild<OwnershipBox*>()->makeConnections(updater);
	provinceTab->findChild<DevBox*>()->makeConnections(updater);


	// Geography Tab
	QWidget* geoTab = new QWidget;
	QVBoxLayout* geoLayout = new QVBoxLayout(geoTab);
	geoLayout->addWidget(new QPushButton("Geo Button"));
	this->addTab(geoTab, "Geography");

	// Country tab
	QWidget* countryTab = new QWidget;
	QVBoxLayout* countryLayout = new QVBoxLayout(countryTab);
	countryLayout->addWidget(new QPushButton("Country Button"));
	this->addTab(countryTab, "Country");

	// TradeNodes tab
	QWidget* tradeTab = new QWidget;
	QVBoxLayout* tradeLayout = new QVBoxLayout(tradeTab);
	tradeLayout->addWidget(new QPushButton("Trade Node 1"));
	this->addTab(tradeTab, "TradeNodes");

	// MacroTools tab
	QWidget* macroTab = new QWidget;
	QVBoxLayout* macroLayout = new QVBoxLayout(macroTab);
	macroLayout->addWidget(new QPushButton("Tool 1"));
	this->addTab(macroTab, "MacroTools");

	// BufferedChanges tab
	//QWidget* bufferTab = new QWidget;
	//QVBoxLayout* bufferLayout = new QVBoxLayout(bufferTab);
	//
	//bufferLayout->addWidget([this] {
	//	QPushButton* l = new QPushButton("Cancel Changes");
	//	connect(l, &QPushButton::clicked, this, [this]() {
	//			for (auto fileData : activeChanges) {
	//				fileData->clearActiveChangedData();
	//			}
	//			activeChanges.clear();
	//		});
	//	return l;
	//	}());

	//bufferLayout->addWidget([this] {
	//	QPushButton* l = new QPushButton("Save Changes");
	//	connect(l, &QPushButton::clicked, this, [this]() {
	//			for (auto fileData : activeChanges) {
	//				fileData->writeIntoFile();
	//				fileData->resetData();
	//				//fileData->resetDataAfterSave<Eu4::GeoPolData>([](Eu4::Province& prov) {}, const_cast<Eu4::GeoPolData&>(mRefGeoPolCont));
	//			}
	//		});
	//	return l;
	//	}());
	BufferTab* bufferTab = new BufferTab(activeChanges);
	this->addTab(bufferTab, "BufferedChanges");
	


	connect(this, &QTabWidget::currentChanged, this, 
		[this, bufferTab](int index) {
			switch (index) {
			case 0: {
				loadProvInfo(*currentProv);
				break;
			}
			case 1: {
				break;
			}
			case 2: {
				break;
			}
			case 3: {
				break;
			}
			case 4: {
				break;
			}
			case 5: {
				bufferTab->updateBufferWidgets();
				break;
			}
			}
		});

	
}

void InformationGUI::loadProvInfo(Eu4::Province& province) const
{
	signalLoadInfo(province);
}

void InformationGUI::initialiseWidgetsInfo()
{
	signalInitCultRelInfo(mRefCultRelCont.mCultures,mRefCultRelCont.mReligions);
	signalInitOwnership(mRefCountryContainer.tagToName);
}

void InformationGUI::changeCurrentProv(Eu4::Province& prov) const
{
	currentProv = &prov;
}

void InformationGUI::updateProvinceField(uint16_t Eu4::Province::* memberPtr, const std::string& newData)
{
	currentProv->updateField(memberPtr, newData);
	activeChanges.insert(currentProv->mFileData);
	
	if (newData.empty() || newData[0] == '0') {
		currentProv->scheduleDelete(memberPtr);
	}
}


