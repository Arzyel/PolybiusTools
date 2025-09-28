#include "ProvinceInfoBox.h"

ProvinceInfoBox::ProvinceInfoBox(QWidget* parent)
	:QWidget(parent)
{
	loadWidgets();
}

void ProvinceInfoBox::loadWidgets()
{
	QVBoxLayout* provinceLayout = new QVBoxLayout(this);
	QGroupBox* provinceBox = new QGroupBox("Technical Info");
	QHBoxLayout* provinceBoxLayout = new QHBoxLayout(provinceBox);
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("ID :");
		l->setObjectName("ID");
		return l;
		}());
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("R : ");
		l->setObjectName("R");
		return l;
		}());
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("G : ");
		l->setObjectName("G");
		return l;
		}());
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("B : ");
		l->setObjectName("B");
		return l;
		}());
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("Water ? : ");
		l->setObjectName("W");
		return l;
		}());

	QGroupBox* provinceInfoBox = new QGroupBox("Pro");


	provinceLayout->addWidget(provinceBox, 1);
	provinceLayout->addWidget(provinceInfoBox, 9);
}

void ProvinceInfoBox::loadProvInfo(const Location& location)
{
	this->findChild<QLabel*>("ID")->setText(("ID : " + location.mEu4UID).c_str());
	auto rgb = location.depackRGB(location.RGBValue);
	this->findChild<QLabel*>("R")->setText(("R : " + std::to_string(rgb.at(0))).c_str());
	this->findChild<QLabel*>("G")->setText(("G : " + std::to_string(rgb.at(1))).c_str());
	this->findChild<QLabel*>("B")->setText(("B : " + std::to_string(rgb.at(2))).c_str());
}




