#include "InformationGUI.h"




InformationGUI::InformationGUI(const Eu4::GeoPolData& refGeoPolCont, const CultRelContainer&  refCultRel, const CountryContainer& countryContainer,
	const Eu4::TGContainer& tgContainer, QWidget* parent)
	:QTabWidget(parent), mRefGeoPolCont(refGeoPolCont), mRefCultRelCont(refCultRel), mRefCountryContainer(countryContainer), mRefTGContainer(tgContainer)
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

	QObject::connect(this, &InformationGUI::signalInitOwnership,
		provinceTab->findChild<CoresBox*>(), &CoresBox::initializeData);
	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab->findChild<CoresBox*>(), &CoresBox::loadProvInfo);

	QObject::connect(this, &InformationGUI::signalInitTradeGood,
		provinceTab->findChild<TradeBox*>(), &TradeBox::initializeData);
	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab->findChild<TradeBox*>(), &TradeBox::loadProvInfo);


	auto updater = [this](uint16_t Eu4::Province::* memberPtr, const std::string& newData) {
		updateProvinceField(memberPtr, newData);
		};

	provinceTab->findChild<CultRelBox*>()->makeConnections(updater);
	provinceTab->findChild<OwnershipBox*>()->makeConnections(updater);
	provinceTab->findChild<DevBox*>()->makeConnections(updater);
	provinceTab->findChild<CoresBox*>()->makeConnections(updater);
	provinceTab->findChild<TradeBox*>()->makeConnections(updater);


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

	//Buffer Tab
	BufferTab* bufferTab = new BufferTab(activeChanges);
	this->addTab(bufferTab, "BufferedChanges");
	
	this->setTabEnabled(1,false);
	this->setTabEnabled(2,false);
	this->setTabEnabled(3,false);
	this->setTabEnabled(4,false);


	connect(this, &QTabWidget::currentChanged, this, 
		[this, bufferTab](int index) {
			switch (index) {
			case 0: {
				if (mSelectedProv.size() == 1) {
					loadProvInfo(**(mSelectedProv.begin()));
				}
				else {
					Eu4::Province prov = Eu4::Province();
					loadProvInfo(prov);
				}
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
	signalInitCultRelInfo(mRefCultRelCont.getAllCultures(), mRefCultRelCont.getAllReligions());
	signalInitOwnership(mRefCountryContainer.tagToName);
	signalInitTradeGood(mRefTGContainer.getAllTradeGoods());
}

void InformationGUI::changeCurrentProv(Eu4::Province& prov) const
{
	currentProv = &prov;
}

void InformationGUI::updateProvinceField(uint16_t Eu4::Province::* memberPtr, const std::string& newData)
{

	for(Eu4::Province* prov : mSelectedProv){
		prov->updateField(memberPtr, newData);
		activeChanges.insert(prov->mFileData);
	
		if (newData.empty() || newData[0] == '0') {
			prov->scheduleDelete(memberPtr);
		}
		
	}
	/*currentProv->updateField(memberPtr, newData);
	activeChanges.insert(currentProv->mFileData);
	
	if (newData.empty() || newData[0] == '0') {
		currentProv->scheduleDelete(memberPtr);
	}*/
}

void InformationGUI::addActiveSelection(Eu4::Province& prov) const
{
	mSelectedProv.insert(&prov);
}

void InformationGUI::clearActiveSelection() const
{
	mSelectedProv.clear();
}

bool InformationGUI::delInSelection(Eu4::Province& prov) const
{
	return mSelectedProv.erase(&prov);
}


