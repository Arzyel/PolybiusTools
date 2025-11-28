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
	taxSpinBox->setMaximum(999);
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
	prodSpinBox->setMaximum(999);
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
	manSpinBox->setMaximum(999);
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

void DevBox::loadDevInfo(Eu4::Province& province)
{
	DM::FileData& fileData = *(province.mFileData);

	uint16_t tax = 0;
	uint16_t prod = 0;
	uint16_t manpower = 0;
	
	if (province.mBaseTax != UINT16_MAX) {
		tax = fileData.mDataTokens[province.mBaseTax].getCurrent_uint16_t();
	}
	if (province.mBaseProduction != UINT16_MAX) {
		prod = fileData.mDataTokens[province.mBaseProduction].getCurrent_uint16_t();
	}
	if (province.mBaseManpower != UINT16_MAX) {
		manpower = fileData.mDataTokens[province.mBaseManpower].getCurrent_uint16_t();
	}
	
	taxSpinBox->setValue(tax);
	prodSpinBox->setValue(prod);
	manSpinBox->setValue(manpower);
}


void DevBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable) {
	connect(taxSpinBox, &QSpinBox::editingFinished,
		this,
		[this, callable]() {
			int value = taxSpinBox->value();
			char buf[3];
			int len = 0;
			if (value >= 100) {
				buf[len++] = '0' + (value / 100);
				buf[len++] = '0' + (value / 10 % 10);
				buf[len++] = '0' + (value % 10);
			}
			else if (value >= 10) {
				buf[len++] = '0' + (value / 10);
				buf[len++] = '0' + (value % 10);
			}
			else {
				buf[len++] = '0' + value;
			}
			callable(&Eu4::Province::mBaseTax, std::string(buf,len));
		});

	connect(prodSpinBox, &QSpinBox::editingFinished,
		this,
		[this, callable]() {
			int value = prodSpinBox->value();
			char buf[3];
			int len = 0;
			if (value >= 100) {
				buf[len++] = '0' + (value / 100);
				buf[len++] = '0' + (value / 10 % 10);
				buf[len++] = '0' + (value % 10);
			}
			else if (value >= 10) {
				buf[len++] = '0' + (value / 10);
				buf[len++] = '0' + (value % 10);
			}
			else {
				buf[len++] = '0' + value;
			}
			callable(&Eu4::Province::mBaseProduction, std::string(buf,len));
		});

	connect(manSpinBox, &QSpinBox::editingFinished,
		this,
		[this, callable]() {
			int value = manSpinBox->value();
			char buf[3];
			int len = 0;
			if (value >= 100) {
				buf[len++] = '0' + (value / 100);
				buf[len++] = '0' + (value / 10 % 10);
				buf[len++] = '0' + (value % 10);
			}
			else if (value >= 10) {
				buf[len++] = '0' + (value / 10);
				buf[len++] = '0' + (value % 10);
			}
			else {
				buf[len++] = '0' + value;
			}
			callable(&Eu4::Province::mBaseManpower, std::string(buf,len));
		});
}
