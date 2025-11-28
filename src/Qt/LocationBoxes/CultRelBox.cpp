#include "CultRelBox.h"

CultRelBox::CultRelBox(const QString& title, QWidget* parent)
	:QGroupBox(title, parent)
{
	loadWidget();
}

void CultRelBox::loadWidget() {
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	this->setLayout(mainLayout);

	QWidget* leftPart = new QWidget(this);
	QVBoxLayout* leftPartLayout = new QVBoxLayout(leftPart);
	QWidget* rightPart = new QWidget(this);
	QVBoxLayout* rightPartLayout = new QVBoxLayout(rightPart);

	
	QWidget* cultContainer = new QWidget(leftPart);
	QHBoxLayout* cultContainerLayout = new QHBoxLayout(cultContainer);
	QLabel* cultLabel = new QLabel("Culture  : ",cultContainer);
	cultCBox = new QComboBox(cultContainer);
	cultCBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	cultCBox->setMinimumWidth(0);
	cultContainerLayout->addWidget(cultLabel);
	cultContainerLayout->addWidget(cultCBox);

	
	QWidget* relContainer = new QWidget(leftPart);
	QHBoxLayout* relContainerLayout = new QHBoxLayout(relContainer);
	QLabel* relLabel = new QLabel("Religion : ", relContainer);
	relCBox = new QComboBox(relContainer);
	relCBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	relCBox->setMinimumWidth(cultCBox->width());
	relContainerLayout->addWidget(relLabel);
	relContainerLayout->addWidget(relCBox);


	leftPartLayout->addWidget(cultContainer);
	leftPartLayout->addWidget(relContainer);

	mainLayout->addWidget(leftPart,19);
	mainLayout->addWidget(rightPart,1);

}

void CultRelBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable)
{
	connect(cultCBox, &QComboBox::activated,
		this,
		[this, callable](int index) {
			QString text = cultCBox->itemText(index);
			QByteArray data = text.toUtf8();
			callable(&Eu4::Province::mCultureID2, std::string(data.constData(), data.size()));
		});
	connect(relCBox, &QComboBox::activated,
		this,
		[this, callable](int index) {
			QString text = relCBox->itemText(index);
			QByteArray data = text.toUtf8();
			callable(&Eu4::Province::mReligionID2, std::string(data.constData(), data.size()));
		});
}






void CultRelBox::initializeData(const std::unordered_map<std::string, sCulture>& cultureData, const std::unordered_map<std::string, sReligion>& religionData)
{
	cultCBox->clear();
	relCBox->clear();
	for (const auto& [key, data] : cultureData) {
		cultCBox->addItem(QString::fromStdString(key));
	}
	for (const auto& [key, data] : religionData) {
		relCBox->addItem(QString::fromStdString(key));
	}
}

void CultRelBox::loadProvInfo(Eu4::Province& province) {
	if (province.mCultureID2 != UINT16_MAX) {
		int index = cultCBox->findText(province.mFileData->mDataTokens[province.mCultureID2].getCurrentName().c_str());
		cultCBox->setCurrentIndex(index);
	}
	else {
		cultCBox->setCurrentIndex(-1);
	}
	if (province.mReligionID2 != UINT16_MAX) {
		int indexRel = relCBox->findText(province.mFileData->mDataTokens[province.mReligionID2].getCurrentName().c_str());
		relCBox->setCurrentIndex(indexRel);
	}
	else {
		relCBox->setCurrentIndex(-1);

	}
}
