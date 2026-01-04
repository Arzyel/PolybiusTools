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
	table->setHorizontalHeaderLabels({ "Icon", "Tag", "Delete" });
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

	box = new QComboBox(footer);
	box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	box->setMinimumWidth(0);
	footerLayout->addWidget(box);

	addBtn = new QPushButton("Add Core");
	footerLayout->addWidget(addBtn);
}

void CoresBox::initializeData(const std::unordered_map<std::string, std::string>& data) {
	box->clear();
	for (const auto& [key, data] : data) {
		std::string tag_name = key + " ; " + data;
		box->addItem(QString::fromStdString(tag_name));
	}
}

void CoresBox::loadProvInfo(Eu4::Province& province) {
	table->setUpdatesEnabled(false);
	table->setRowCount(0);
	for (const auto& coreID : province.mCoresID) {
		//hack to not create the row when empty probably should raise an exception when it receives a wrong tag
		const QString tag = QString::fromStdString(province.mFileData->mDataTokens[coreID].getCurrentName());

		if (!province.mFileData->mDataTokens.at(coreID).erase && tag.size() == 3) {
			addRow(tag);
		}
	}
	table->setUpdatesEnabled(true);
}

void CoresBox::addRow(const QString& tag)
{
	int row = table->rowCount();
	table->insertRow(row);
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
		if (table->findItems(box->itemText(box->currentIndex()).left(3), Qt::MatchExactly).size() == 0) {
			QString text = box->itemText(box->currentIndex()).left(3);
			QByteArray data = text.toUtf8();
			addRow(text);
			callable(reinterpret_cast<uint16_t Eu4::Province::*>(&Eu4::Province::mCoresID) , std::string(data.constData(), 3));
		}

		});
}