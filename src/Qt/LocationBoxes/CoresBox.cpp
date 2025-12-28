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

	QPushButton* addBtn = new QPushButton("Add Core");
	connect(addBtn, &QPushButton::clicked, this, [this]() {

		if (table->findItems(box->itemText(box->currentIndex()).left(3), Qt::MatchExactly).size() == 0) {
			addRow(box->itemText(box->currentIndex()).left(3));
		}

		});
	footerLayout->addWidget(addBtn);
}

void CoresBox::initializeData(const std::unordered_map<std::string, std::string>& data) {
	box->clear();
	box->addItem("");
	for (const auto& [key, data] : data) {
		std::string tag_name = key + " ; " + data;
		box->addItem(QString::fromStdString(tag_name));
	}

}

void CoresBox::loadProvInfo(Eu4::Province& province) {
	// add logic to load all cores into the tab widget
	table->setUpdatesEnabled(false);
	table->setRowCount(0);

	for (const auto& coreID : province.mCoresID2) {
		addRow(QString::fromUtf8(province.mFileData->mDataTokens[coreID].mPtrStart, province.mFileData->mDataTokens[coreID].mLength));
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
		table->removeRow(row);
		});
	table->setCellWidget(row, 2, del);

}

void CoresBox::makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable)
{
	//adapat when addubg a core on button push so it adds into the qtab and sends the correct data to the callable
	/*connect(ownerBox, &QComboBox::activated,
		this,
		[this, callable](int index) {
			QString text = ownerBox->itemText(index);
			QByteArray data = text.toUtf8();
			callable(&Eu4::Province::mOwnerID2, std::string(data.constData(), 3));
		});*/
	
}