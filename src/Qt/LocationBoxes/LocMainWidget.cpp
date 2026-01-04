#include "LocMainWidget.h"

LocMainWidget::LocMainWidget(QWidget* parent)
	:QWidget(parent)
{
	loadWidgets();
}

void LocMainWidget::loadWidgets()
{
	QVBoxLayout* provinceLayout = new QVBoxLayout(this);
	QGroupBox* provinceBox = new QGroupBox("Technical Info");
	QHBoxLayout* provinceBoxLayout = new QHBoxLayout(provinceBox);
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("ID :");
		l->setObjectName("ID");
		l->setFixedWidth(50);
		return l;
		}());
	provinceBoxLayout->addWidget([] {
		QLabel* l = new QLabel("Name :");
		l->setObjectName("Name");
		l->setFixedWidth(200);
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

	QGroupBox* provinceInfoBox = new LocInfoBoxes("Province Information");

	provinceLayout->addWidget(provinceBox, 1);
	provinceLayout->addWidget(provinceInfoBox, 9);
}

void LocMainWidget::loadProvInfo(Eu4::Province& province)
{
	this->findChild<QLabel*>("ID")->setText(("ID : " + std::to_string(province.mUID)).c_str());
	this->findChild<QLabel*>("Name")->setText(("Name : " + province.mName).c_str());
	auto rgb = province.depackRGB();
	this->findChild<QLabel*>("R")->setText(("R : " + std::to_string(rgb.at(0))).c_str());
	this->findChild<QLabel*>("G")->setText(("G : " + std::to_string(rgb.at(1))).c_str());
	this->findChild<QLabel*>("B")->setText(("B : " + std::to_string(rgb.at(2))).c_str());
	this->findChild<QLabel*>("W")->setText(("Water : " + std::to_string(province.isWater)).c_str());
}




