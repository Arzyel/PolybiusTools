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
	connect(cultCBox, &QComboBox::activated,
		this,
		[this](int index) {
			if (!currentProvince) return;
			QString text = cultCBox->itemText(index);
			QByteArray data = text.toUtf8();
			currentProvince->mFileData->mDataTokens[currentProvince->mCultureID2].mNewData = std::string(data.constData(), data.size());
		});




	
	QWidget* relContainer = new QWidget(leftPart);
	QHBoxLayout* relContainerLayout = new QHBoxLayout(relContainer);
	QLabel* relLabel = new QLabel("Religion : ", relContainer);
	relCBox = new QComboBox(relContainer);
	relCBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	relCBox->setMinimumWidth(cultCBox->width());
	relContainerLayout->addWidget(relLabel);
	relContainerLayout->addWidget(relCBox);
	connect(relCBox, &QComboBox::activated,
		this,
		[this](int index) {
			if (!currentProvince) return;
			QString text = relCBox->itemText(index);
			QByteArray data = text.toUtf8();
			currentProvince->mFileData->mDataTokens[currentProvince->mReligionID2].mNewData = std::string(data.constData(), data.size());
		});

	leftPartLayout->addWidget(cultContainer);
	leftPartLayout->addWidget(relContainer);

	mainLayout->addWidget(leftPart,19);
	mainLayout->addWidget(rightPart,1);

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
	int index = cultCBox->findText(province.mFileData->mDataTokens[province.mCultureID2].getCurrentName().c_str());
	cultCBox->setCurrentIndex(index);

	int indexRel = relCBox->findText(province.mFileData->mDataTokens[province.mReligionID2].getCurrentName().c_str());
	relCBox->setCurrentIndex(indexRel);

	currentProvince = &province;
}

void CultRelBox::onCultureChanged(const QString& text)
{
	if (!currentProvince) return;
	/*std::string newData = text.toUtf8().constData();
	currentProvince->mFileData->mDataTokens[currentProvince->mCultureID2].mNewData = std::move(newData);*/
}
