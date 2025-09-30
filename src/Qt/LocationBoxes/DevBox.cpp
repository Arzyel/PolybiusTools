#include "DevBox.h"

DevBox::~DevBox()
{
	delete taxSpinBox;
	delete prodSpinBox;
	delete manSpinBox;
}

DevBox::DevBox(const QString& title, QWidget* parent)
	:QGroupBox(title, parent), gen(std::random_device{}()), distLow(0, 3), distMid(2, 6), distHigh(5, 10)
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



	// MiddleColumn
	QPushButton* bAddDevAll = new QPushButton("Add dev all", middlePart);
	QPushButton* bSubDevAll = new QPushButton("Sub dev all", middlePart);
	QObject::connect(bAddDevAll, &QPushButton::clicked, [&]() {
		taxSpinBox->setValue(taxSpinBox->value() + 1);
		prodSpinBox->setValue(prodSpinBox->value() + 1);
		manSpinBox->setValue(manSpinBox->value() + 1);
		});
	QObject::connect(bSubDevAll, &QPushButton::clicked, [&]() {
		taxSpinBox->setValue(taxSpinBox->value() - 1);
		prodSpinBox->setValue(prodSpinBox->value() - 1);
		manSpinBox->setValue(manSpinBox->value() - 1);
		});

	middlePartLayout->addWidget(bAddDevAll);
	middlePartLayout->addWidget(bSubDevAll);

	// right column
	QPushButton* bRndLowDev = new QPushButton("Rnd Low Dev", rightPart);
	QObject::connect(bRndLowDev, &QPushButton::clicked, [&]() {
		taxSpinBox->setValue(distLow(gen));
		prodSpinBox->setValue(distLow(gen));
		manSpinBox->setValue(distLow(gen));
		});
	QPushButton* bRndMidDev = new QPushButton("Rnd Mid Dev", rightPart);
	QObject::connect(bRndMidDev, &QPushButton::clicked, [&]() {
		taxSpinBox->setValue(distMid(gen));
		prodSpinBox->setValue(distMid(gen));
		manSpinBox->setValue(distMid(gen));
		});
	QPushButton* bRndHighDev = new QPushButton("Rnd High Dev", rightPart);
	QObject::connect(bRndHighDev, &QPushButton::clicked, [&]() {
		taxSpinBox->setValue(distHigh(gen));
		prodSpinBox->setValue(distHigh(gen));
		manSpinBox->setValue(distHigh(gen));
		});



	rightPartLayout->addWidget(bRndLowDev);
	rightPartLayout->addWidget(bRndMidDev);
	rightPartLayout->addWidget(bRndHighDev);

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
