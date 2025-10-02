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
	cultCBox->addItems({ "french", "english","englisher","englisherer""englishman", "german"});
	cultCBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	cultCBox->setMinimumWidth(0);
	cultContainerLayout->addWidget(cultLabel);
	cultContainerLayout->addWidget(cultCBox);

	
	QWidget* relContainer = new QWidget(leftPart);
	QHBoxLayout* relContainerLayout = new QHBoxLayout(relContainer);
	QLabel* relLabel = new QLabel("Religion : ", relContainer);
	relCBox = new QComboBox(relContainer);
	relCBox->addItems({ "christian", "islam", "dharmic"});
	relCBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	relCBox->setMinimumWidth(cultCBox->width());
	relContainerLayout->addWidget(relLabel);
	relContainerLayout->addWidget(relCBox);


	leftPartLayout->addWidget(cultContainer);
	leftPartLayout->addWidget(relContainer);

	mainLayout->addWidget(leftPart,19);
	mainLayout->addWidget(rightPart,1);
}

//void CultRelBox::initializeData(const std::unordered_map<std::string, sCulture>& cultureData, const std::unordered_map<std::string, sReligion>& religionData)
//{
//	cultCBox->clear();
//	for (const auto& [key, data] : cultureData) {
//		cultCBox->addItem(QString::fromStdString(key));
//	}
//}
