#include "InformationGUI.h"




InformationGUI::InformationGUI(const GeoPolContainers& refGeoPolCont, QWidget* parent)
	:QTabWidget(parent), mRefGeoPolCont(refGeoPolCont)
{

}

void InformationGUI::loadWidgets()
{
	// Province tab with layout and optional group box
	ProvinceInfoBox* provinceTab = new ProvinceInfoBox();
	this->addTab(provinceTab, "Province");

	QObject::connect(this, &InformationGUI::signalLoadInfo,
		provinceTab, &ProvinceInfoBox::loadProvInfo);

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
