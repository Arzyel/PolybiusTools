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
	table->horizontalHeader()->setStretchLastSection(true);
	table->verticalHeader()->setVisible(false);
	table->setSelectionMode(QAbstractItemView::NoSelection);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);

	table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	table->verticalHeader()->setDefaultSectionSize(24);
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
		box->setCurrentIndex(box->currentIndex() + 1);
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
	table->setRowCount(province.mCoresID2.size());

	int row = 0;
	for (const auto& coreID : province.mCoresID2) {

		//QTableWidgetItem* icon;
		QTableWidgetItem* tag = new QTableWidgetItem(
			QString::fromUtf8(province.mFileData->mDataTokens[coreID].mPtrStart, province.mFileData->mDataTokens[coreID].mLength)
		);
		//QTableWidgetItem* delBtn;
		
		//table->setItem(row, 0, icon);
		table->setItem(row, 1, tag);
		//table->setItem(row, 2, delBtn);
		++row;
	}
	table->setUpdatesEnabled(true);
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