#include "InformationGUI.h"




InformationGUI::InformationGUI(const GeoPolContainers& refGeoPolCont, const CultRelContainer&  refCultRel, QWidget* parent)
	:QTabWidget(parent), mRefGeoPolCont(refGeoPolCont), mRefCultRelCont(refCultRel)
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
	
	//QObject::connect(this, &InformationGUI::sendCultRelInfo,
	//	provinceTab->findChild<CultRelBox*>(), &CultRelBox::initializeData);

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
	QWidget* bufferTab = new QWidget;
	QVBoxLayout* bufferLayout = new QVBoxLayout(bufferTab);
	bufferLayout->addWidget(new QPushButton("Change 1"));
	this->addTab(bufferTab, "BufferedChanges");
	

	
}

void InformationGUI::loadProvInfo(const Location& location) const
{
	signalLoadInfo(location);
}

//void InformationGUI::sendCultRelInfo(const std::unordered_map<std::string, sCulture>& cultureData, const std::unordered_map<std::string, sReligion>& religionData)
//{
//}
