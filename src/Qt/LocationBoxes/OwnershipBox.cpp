#include "OwnershipBox.h"

OwnershipBox::OwnershipBox(const QString& title, QWidget* parent) 
	:QGroupBox(title, parent)
{
	loadWidget();
}

void OwnershipBox::loadWidget() {
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	this->setLayout(mainLayout);

	QWidget* leftPart = new QWidget(this);
	QVBoxLayout* leftPartLayout = new QVBoxLayout(leftPart);
	QWidget* rightPart = new QWidget(this);
	QVBoxLayout* rightPartLayout = new QVBoxLayout(rightPart);


	QWidget* ownerContainer = new QWidget(leftPart);
	QHBoxLayout* ownerContainerLayout = new QHBoxLayout(ownerContainer);
	QLabel* ownerLabel = new QLabel("Owner : ", ownerContainer);
	ownerBox = new QComboBox(ownerContainer);
	ownerBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ownerBox->setMinimumWidth(0);
	ownerContainerLayout->addWidget(ownerLabel);
	ownerContainerLayout->addWidget(ownerBox);


	QWidget* controllerContainer = new QWidget(leftPart);
	QHBoxLayout* controllerContainerLayout = new QHBoxLayout(controllerContainer);
	QLabel* controllerLabel = new QLabel("Controller : ", controllerContainer);
	controllerBox = new QComboBox(controllerContainer);
	controllerBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	controllerBox->setMinimumWidth(controllerBox->width());
	controllerContainerLayout->addWidget(controllerLabel);
	controllerContainerLayout->addWidget(controllerBox);
	controllerBox->setCurrentIndex(0);

	leftPartLayout->addWidget(ownerContainer);
	leftPartLayout->addWidget(controllerContainer);

	mainLayout->addWidget(leftPart, 19);
	mainLayout->addWidget(rightPart, 1);
}

void OwnershipBox::initializeData(const std::unordered_map<std::string, std::string>& data) {
	ownerBox->clear();
	controllerBox->clear();
	for (const auto& [key, data] : data) {

		std::string tag_name = key + " ; " + data;
		ownerBox->addItem(QString::fromStdString(tag_name));
		controllerBox->addItem(QString::fromStdString(tag_name));
	}

}

void OwnershipBox::loadProvInfo(Eu4::Province& province) {
	int index = -1;
	if (province.mOwnerID2 != UINT16_MAX) {
		index = ownerBox->findText(province.mFileData->mDataTokens[province.mOwnerID2].getCurrentName().c_str(), Qt::MatchStartsWith);
	}
	ownerBox->setCurrentIndex(index);

	int index2 = -1;
	if (province.mControllerID2 != UINT16_MAX) {
		index2 = ownerBox->findText(province.mFileData->mDataTokens[province.mControllerID2].getCurrentName().c_str(), Qt::MatchStartsWith);
	}
	controllerBox->setCurrentIndex(index2);
}

void OwnershipBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable)
{
	connect(ownerBox, &QComboBox::activated,
		this,
		[this, callable](int index) {
			QString text = ownerBox->itemText(index);
			QByteArray data = text.toUtf8();
			callable(&Eu4::Province::mOwnerID2, std::string(data.constData(), data.size()));
		});
	connect(controllerBox, &QComboBox::activated,
		this,
		[this, callable](int index) {
			QString text = controllerBox->itemText(index);
			QByteArray data = text.toUtf8();
			callable(&Eu4::Province::mControllerID2, std::string(data.constData(), data.size()));
		});
}