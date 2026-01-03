#include "TradeBox.h"

TradeBox::TradeBox(const QString& title, QWidget* parent)
	:QGroupBox(title,parent)
{
	loadWidget();
}

void TradeBox::loadWidget()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	this->setLayout(mainLayout);

	QWidget* leftPart = new QWidget(this);
	QVBoxLayout* leftPartLayout = new QVBoxLayout(leftPart);
	QWidget* rightPart = new QWidget(this);
	QVBoxLayout* rightPartLayout = new QVBoxLayout(rightPart);


	QWidget* cultContainer = new QWidget(leftPart);
	QHBoxLayout* cultContainerLayout = new QHBoxLayout(cultContainer);
	QLabel* TGLbl = new QLabel("Trade Good  : ", cultContainer);
	mTGBox = new QComboBox(cultContainer);
	mTGBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	mTGBox->setMinimumWidth(0);
	cultContainerLayout->addWidget(TGLbl);
	cultContainerLayout->addWidget(mTGBox);

	leftPartLayout->addWidget(cultContainer);

	mainLayout->addWidget(leftPart, 19);
	mainLayout->addWidget(rightPart, 1);

}

void TradeBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable)
{
	connect(mTGBox, &QComboBox::activated,
		this,
		[this, callable](int index) {
			QString text = mTGBox->itemText(index);
			QByteArray data = text.toUtf8();
			callable(&Eu4::Province::mTradeGood, std::string(data.constData(), data.size()));
		});
}

void TradeBox::initializeData(const std::vector<std::string_view>& tradeGoods)
{
	mTGBox->clear();
	mTGBox->addItem("");
	for (const auto& tg : tradeGoods) {
		mTGBox->addItem(QString::fromUtf8(tg.data(), tg.size()));
	}
}

void TradeBox::loadProvInfo(Eu4::Province& province)
{
	if (province.mTradeGood != UINT16_MAX) {
		int index = mTGBox->findText(province.mFileData->mDataTokens[province.mTradeGood].getCurrentName().c_str());
		mTGBox->setCurrentIndex(index);
	}
	else {
		mTGBox->setCurrentIndex(-1);
	}
}


