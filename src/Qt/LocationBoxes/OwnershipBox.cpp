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
	ownerBox->addItems({ "french", "english","englisher","englisherer""englishman", "german" });
	ownerBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ownerBox->setMinimumWidth(0);
	ownerContainerLayout->addWidget(ownerLabel);
	ownerContainerLayout->addWidget(ownerBox);


	QWidget* controllerContainer = new QWidget(leftPart);
	QHBoxLayout* controllerContainerLayout = new QHBoxLayout(controllerContainer);
	QLabel* controllerLabel = new QLabel("Controller : ", controllerContainer);
	controllerBox = new QComboBox(controllerContainer);
	controllerBox->addItems({ "christian", "islam", "dharmic" });
	controllerBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	controllerBox->setMinimumWidth(controllerBox->width());
	controllerContainerLayout->addWidget(controllerLabel);
	controllerContainerLayout->addWidget(controllerBox);


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