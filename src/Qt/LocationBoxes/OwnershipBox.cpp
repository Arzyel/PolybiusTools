#include "OwnershipBox.h"
#include "CoresBox.h"

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


	QWidget* checkboxContainer = new QWidget(leftPart);
	QHBoxLayout* checkboxContainerLayout = new QHBoxLayout(checkboxContainer);
	QLabel* checkboxLabel = new QLabel("Link Selection of owner and controller : ", checkboxContainer);
	linkSelectionBox = new QCheckBox(checkboxContainer);
	linkSelectionBox->setChecked(true);
	checkboxContainerLayout->addWidget(checkboxLabel);
	checkboxContainerLayout->addWidget(linkSelectionBox);

	QWidget* ownerContainer = new QWidget(leftPart);
	QHBoxLayout* ownerContainerLayout = new QHBoxLayout(ownerContainer);
	QLabel* ownerLabel = new QLabel("Owner :      ", ownerContainer);
	ownerBoxName = new QComboBox(ownerContainer);
	ownerBoxName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ownerBoxName->setMinimumWidth(0);
	ownerBoxTag = new QComboBox(ownerContainer);
	ownerBoxTag->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	ownerBoxTag->setMinimumWidth(0);
	ownerContainerLayout->addWidget(ownerLabel);
	ownerContainerLayout->addWidget(ownerBoxName);
	ownerContainerLayout->addWidget(ownerBoxTag);

	QWidget* controllerContainer = new QWidget(leftPart);
	QHBoxLayout* controllerContainerLayout = new QHBoxLayout(controllerContainer);
	QLabel* controllerLabel = new QLabel("Controller : ", controllerContainer);
	controllerBoxTag = new QComboBox(controllerContainer);
	controllerBoxTag->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	controllerBoxTag->setMinimumWidth(controllerBoxTag->width());
	controllerBoxName = new QComboBox(controllerContainer);
	controllerBoxName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	controllerBoxName->setMinimumWidth(controllerBoxTag->width());
	controllerContainerLayout->addWidget(controllerLabel);
	controllerContainerLayout->addWidget(controllerBoxName);
	controllerContainerLayout->addWidget(controllerBoxTag);

	leftPartLayout->addWidget(ownerContainer);
	leftPartLayout->addWidget(controllerContainer);
	leftPartLayout->addWidget(checkboxContainer);

	mainLayout->addWidget(leftPart, 19);



	connect(ownerBoxName, &QComboBox::currentIndexChanged, ownerBoxTag, [this](int index) {
		QSignalBlocker block(ownerBoxTag);
		this->ownerBoxTag->setCurrentIndex(index);
		if (linkSelectionBox->isChecked()) {
			QSignalBlocker block1(controllerBoxTag);
			QSignalBlocker block2(controllerBoxName);
			this->controllerBoxTag->setCurrentIndex(index);
			this->controllerBoxName->setCurrentIndex(index);
		}
		});
	connect(ownerBoxTag, &QComboBox::currentIndexChanged, ownerBoxName, [this](int index) {
		QSignalBlocker block(ownerBoxName);
		this->ownerBoxName->setCurrentIndex(index);
		if (linkSelectionBox->isChecked()) {
			QSignalBlocker block1(controllerBoxTag);
			QSignalBlocker block2(controllerBoxName);
			this->controllerBoxTag->setCurrentIndex(index);
			this->controllerBoxName->setCurrentIndex(index);
		}
		});
	connect(controllerBoxTag, &QComboBox::currentIndexChanged, controllerBoxName, [this](int index) {
		QSignalBlocker block(controllerBoxName);
		this->controllerBoxName->setCurrentIndex(index);
		if (linkSelectionBox->isChecked()) {
			QSignalBlocker block1(ownerBoxName);
			QSignalBlocker block2(ownerBoxTag);
			this->ownerBoxName->setCurrentIndex(index);
			this->ownerBoxTag->setCurrentIndex(index);
		}
		});
	connect(controllerBoxName, &QComboBox::currentIndexChanged, controllerBoxTag, [this](int index) {
		QSignalBlocker block(controllerBoxTag);
		this->controllerBoxTag->setCurrentIndex(index);
		if (linkSelectionBox->isChecked()) {
			QSignalBlocker block1(ownerBoxName);
			QSignalBlocker block2(ownerBoxTag);
			this->ownerBoxName->setCurrentIndex(index);
			this->ownerBoxTag->setCurrentIndex(index);
		}
		});
}

void OwnershipBox::initializeData(const std::unordered_map<std::string, std::string>& data) {
	ownerBoxName->clear();
	ownerBoxTag->clear();
	controllerBoxTag->clear();
	controllerBoxName->clear();
	ownerBoxName->addItem("");
	ownerBoxTag->addItem("");
	controllerBoxTag->addItem("");
	controllerBoxName->addItem("");
	for (const auto& [tag, name] : data) {
		ownerBoxName->addItem(QString::fromStdString(name));
		ownerBoxTag->addItem(QString::fromStdString(tag));
		controllerBoxTag->addItem(QString::fromStdString(tag));
		controllerBoxName->addItem(QString::fromStdString(name));
	}

}

void OwnershipBox::loadProvInfo(Eu4::Province& province) {
	bool oldState = linkSelectionBox->isChecked();
	linkSelectionBox->setChecked(false);
	int index = -1;
	if (province.mOwnerID != UINT16_MAX) {
		index = ownerBoxTag->findText(province.mFileData->mDataTokens[province.mOwnerID].getCurrentName().c_str(), Qt::MatchStartsWith);
	}
	ownerBoxTag->setCurrentIndex(index);

	int index2 = -1;
	if (province.mControllerID != UINT16_MAX) {
		index2 = controllerBoxTag->findText(province.mFileData->mDataTokens[province.mControllerID].getCurrentName().c_str(), Qt::MatchStartsWith);
	}
	controllerBoxTag->setCurrentIndex(index2);
	linkSelectionBox->setChecked(oldState);
}

void OwnershipBox::changeOwner(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable, int index)
{
	QString text = ownerBoxTag->itemText(index);
	QByteArray data = text.toUtf8();
	callable(&Eu4::Province::mOwnerID, std::string(data.constData(), 3));
}

void OwnershipBox::changeController(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable, int index)
{
	QString text = controllerBoxTag->itemText(index);
	QByteArray data = text.toUtf8();
	callable(&Eu4::Province::mControllerID, std::string(data.constData(), 3));
}

void OwnershipBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable)
{
	connect(ownerBoxName, &QComboBox::activated,
		this,
		[this, callable](int index) {
			changeOwner(callable, index);
			if (linkSelectionBox->isChecked()) {
				changeController(callable, index);
			}
		});
	connect(ownerBoxTag, &QComboBox::activated,
		this,
		[this, callable](int index) {
			changeOwner(callable, index);
			if (linkSelectionBox->isChecked()) {
				changeController(callable, index);
			}
		});
	connect(controllerBoxTag, &QComboBox::activated,
		this,
		[this, callable](int index) {
			changeController(callable, index);
			if (linkSelectionBox->isChecked()) {
				changeOwner(callable, index);
			}
		});
	connect(controllerBoxName, &QComboBox::activated,
		this,
		[this, callable](int index) {
			changeController(callable, index);
			if (linkSelectionBox->isChecked()) {
				changeOwner(callable, index);
			}
		});
}