#include "DevBox.h"

DevBox::~DevBox()
{
	delete taxSpinBox;
	delete prodSpinBox;
	delete manSpinBox;
}

DevBox::DevBox(const QString& title, QWidget* parent)
	:QGroupBox(title, parent)
{
	loadWidgets();
}

void DevBox::loadWidgets() {
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	this->setLayout(mainLayout);

	QWidget* leftPart = new QWidget(this);
	QVBoxLayout* leftPartLayout = new QVBoxLayout(leftPart);
	QWidget* middlePart = new QWidget(this);
	QVBoxLayout* middlePartLayout = new QVBoxLayout(middlePart);
	QWidget* rightPart = new QWidget(this);
	QVBoxLayout* rightPartLayout = new QVBoxLayout(rightPart);






	//Tax widgets
	QWidget* taxWidget = new QWidget(leftPart);
	QHBoxLayout* taxWidgetLayout = new QHBoxLayout(taxWidget);

	QLabel* taxLabel = new QLabel("Tax : ");
	taxSpinBox = new QSpinBox;
	taxSpinBox->setMinimum(0);
	taxSpinBox->setMaximum(1000);
	taxSpinBox->setValue(0);

	taxWidgetLayout->addWidget(taxLabel);
	taxWidgetLayout->addWidget(taxSpinBox);
	leftPartLayout->addWidget(taxWidget);

	// Prod widgets
	QWidget* prodWidget = new QWidget(leftPart);
	QHBoxLayout* prodWidgetLayout = new QHBoxLayout(prodWidget);

	QLabel* prodLabel = new QLabel("Production : ");
	prodSpinBox = new QSpinBox;
	prodSpinBox->setMinimum(0);
	prodSpinBox->setMaximum(1000);
	prodSpinBox->setValue(0);

	prodWidgetLayout->addWidget(prodLabel);
	prodWidgetLayout->addWidget(prodSpinBox);
	leftPartLayout->addWidget(prodWidget);


	// Man widgets
	QWidget* manWidget = new QWidget(leftPart);
	QHBoxLayout* manWidgetLayout = new QHBoxLayout(manWidget);

	QLabel* manLabel = new QLabel("Manpower : ");
	manSpinBox = new QSpinBox;
	manSpinBox->setMinimum(0);
	manSpinBox->setMaximum(1000);
	manSpinBox->setValue(0);

	manWidgetLayout->addWidget(manLabel);
	manWidgetLayout->addWidget(manSpinBox);
	leftPartLayout->addWidget(manWidget);



	mainLayout->addWidget(leftPart);
	mainLayout->addWidget(middlePart);
	mainLayout->addWidget(rightPart);
	
}

void DevBox::loadDevInfo(const Location& location)
{
	taxSpinBox->setValue(location.mDevelopment.base_tax);
	prodSpinBox->setValue(location.mDevelopment.base_prod);
	manSpinBox->setValue(location.mDevelopment.base_manpower);
}
