#include "CoresBox.h"

CoresBox::CoresBox(const QString& title, QWidget* parent)
	:QGroupBox(title,parent)
{
	loadWidget();
}

void CoresBox::loadWidget()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);

	table = new QTableWidget(this);
	table->setColumnCount(3);
	table->setHorizontalHeaderLabels({ "Name", "Tag", "Delete" });
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	table->verticalHeader()->setVisible(false);
	table->setSelectionMode(QAbstractItemView::NoSelection);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);

	table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	table->verticalHeader()->setDefaultSectionSize(24);
	table->setColumnWidth(2, 55);
	table->setShowGrid(true);

	mainLayout->addWidget(table,9);

	QWidget* footer = new QWidget(this);
	QHBoxLayout* footerLayout = new QHBoxLayout(footer);
	footerLayout->setContentsMargins(2, 2, 2, 2);
	mainLayout->addWidget(footer);

	boxName = new QComboBox(footer);
	boxName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	boxName->setMinimumWidth(0);
	footerLayout->addWidget(boxName);

	boxTag = new QComboBox(footer);
	boxTag->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	boxTag->setMinimumWidth(0);
	footerLayout->addWidget(boxTag);



	connect(boxTag, &QComboBox::currentIndexChanged, boxName, [this](int index) {
		this->boxName->setCurrentIndex(index);
		});

	connect(boxName, &QComboBox::currentIndexChanged, boxTag, [this](int index) {
		this->boxTag->setCurrentIndex(index);
		});



	addBtn = new QPushButton("Add Core");
	footerLayout->addWidget(addBtn);
}

void CoresBox::initializeData(const std::unordered_map<std::string, std::string>& data) {
	boxTag->clear();
	boxName->clear();
	for (const auto& [key, data] : data) {
		boxTag->addItem(QString::fromStdString(key));
		boxName->addItem(QString::fromStdString(data));
	}
}

void CoresBox::loadProvInfo(Eu4::Province& province) {
	table->setUpdatesEnabled(false);
	table->setRowCount(0);
	for (const auto& coreID : province.mCoresID) {
		//hack to not create the row when empty probably should raise an exception when it receives a wrong tag
		const QString tag = QString::fromStdString(province.mFileData->mDataTokens[coreID].getCurrentName());
		
		if (!province.mFileData->mDataTokens.at(coreID).erase && tag.size() == 3) {
			int index = boxTag->findText(tag, Qt::MatchExactly);
			QString name = boxName->itemText(index);
			addRow(name, tag);
		}
	}
	table->setUpdatesEnabled(true);
}

void CoresBox::addRow(const QString& name, const QString& tag)
{
	int row = table->rowCount();
	table->insertRow(row);
	
	QTableWidgetItem* itemName = new QTableWidgetItem(name);
	table->setItem(row, 0, itemName);

	QTableWidgetItem* itemTag = new QTableWidgetItem(tag);
	table->setItem(row, 1, itemTag);

	QPushButton* del = new QPushButton("x");
	del->setIconSize(QSize(16, 16));
	del->setFixedSize(16, 16);
	connect(del, &QPushButton::clicked, this, [this, del]() {
		int row = table->indexAt(del->pos()).row();
		if (row < 0) return;
		QTableWidgetItem* item = table->item(row, 1);
		if (item) {
			callable(reinterpret_cast<uint16_t Eu4::Province::*>(&Eu4::Province::mCoresID), item->text().toStdString());
			table->removeRow(row);
		}
		});
	table->setCellWidget(row, 2, del);
}

void CoresBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable)
{
	this->callable = callable;
	connect(addBtn, &QPushButton::clicked, this, [this, callable]() {
		if (table->findItems(boxTag->itemText(boxTag->currentIndex()).left(3), Qt::MatchExactly).size() == 0) {
			QString text = boxTag->itemText(boxTag->currentIndex()).left(3);
			QByteArray data = text.toUtf8();
			QString name = boxName->currentText();
			addRow(name, text);
			callable(reinterpret_cast<uint16_t Eu4::Province::*>(&Eu4::Province::mCoresID) , std::string(data.constData(), 3));
		}

		});
}