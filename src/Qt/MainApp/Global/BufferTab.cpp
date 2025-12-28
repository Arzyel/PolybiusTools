#include "BufferTab.h"

BufferTab::BufferTab(std::unordered_set<DM::iFileDataBase*>& setAChanges, QWidget* parent)
	:QWidget(parent), activeChanges(setAChanges)
{
	QVBoxLayout* bufferMainLayout = new QVBoxLayout(this);

	//bufferMainLayout->addWidget([this] {
	//	QLabel* tabName = new QLabel;
	//	tabName->setText("Active Changes");
	//	return tabName;
	//	}(),1);

	bufferMainLayout->addWidget([this] {
		mList = new QListWidget(this);
		mList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
		mList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		mList->setSelectionMode(QAbstractItemView::NoSelection);
		mList->verticalScrollBar()->setSingleStep(2);
		mList->setStyleSheet(
			"QListWidget::item { background: transparent; }"
			"QListWidget::item:hover { background: transparent; }"
			"QListWidget::item:selected { background: transparent; }"
		);

		updateBufferWidgets();

		return mList;
		}(), 8);



	bufferMainLayout->addWidget([this] {
		QWidget* container = new QWidget(this);
		QHBoxLayout* hbox = new QHBoxLayout(container);

		hbox->addWidget([this] {
			QPushButton* l = new QPushButton("Cancel All Changes");
			connect(l, &QPushButton::clicked, this, [this]() {
				for (auto fileData : activeChanges) {
					fileData->clearActiveChangedData();
				}
				activeChanges.clear();
				updateBufferWidgets();
				});
			return l;
			}());

		QPushButton* l = new QPushButton("Save All Changes");
		connect(l, &QPushButton::clicked, this, [this]() {
			for (auto fileData : activeChanges) {
				fileData->writeIntoFile();
				fileData->resetData();
			}
			activeChanges.clear();
			this->updateBufferWidgets();
			});
		hbox->addWidget(l);


		return container;
		}(), 1);

}

void BufferTab::loadWidgets()
{

}

void BufferTab::updateBufferWidgets()
{
	QElapsedTimer timer;
	timer.start();

	mList->setUpdatesEnabled(false);

	// Remove widgets no longer in activeChanges
	for (auto it = mWidgetCache.begin(); it != mWidgetCache.end(); ) {
		if (activeChanges.find(it->first) == activeChanges.end()) {
			delete it->second.item;
			it = mWidgetCache.erase(it);
		}
		else {
			++it;
		}
	}

	// Update or create widgets
	for (auto fileData : activeChanges) {
		auto it = mWidgetCache.find(fileData);
		if (it != mWidgetCache.end()) {
			// Only update if data actually changed
			updateCachedWidgetSmart(fileData, it->second);
		}
		else {
			mWidgetCache[fileData] = createWidgetForFileData(fileData);
		}
	}

	mList->setUpdatesEnabled(true);

	qDebug() << "Total update time:" << timer.elapsed() << "ms";

	/*for (auto fileData : activeChanges) {
		QListWidgetItem* item = new QListWidgetItem(mList);
		item->setSizeHint(QSize(0, 250));
		auto [ptr, len] = fileData->getFileName();
		QGroupBox* group = new QGroupBox(QString::fromUtf8(ptr, len));
		QVBoxLayout* groupLayout = new QVBoxLayout(group);

		QWidget* header = new QWidget(group);
		QHBoxLayout* headerLayout = new QHBoxLayout(header);
		headerLayout->addWidget(new QLabel("Field", header));
		headerLayout->addWidget(new QLabel("Original Value", header));
		headerLayout->addWidget(new QLabel("Modified Value", header));
		groupLayout->addWidget(header);

		QListWidget* list = new QListWidget(group);
		list->setSelectionMode(QAbstractItemView::NoSelection);
		list->verticalScrollBar()->setSingleStep(2);
		groupLayout->addWidget(list);

		QWidget* footer = new QWidget(group);
		QHBoxLayout* footerLayout = new QHBoxLayout(footer);
		
		QPushButton* cancelBtn = new QPushButton("Cancel");
		connect(cancelBtn, &QPushButton::clicked, this, [this, fileData]() {
			fileData->clearActiveChangedData();
			activeChanges.erase(fileData);
			updateBufferWidgets();
			});
		footerLayout->addWidget(cancelBtn);

		QPushButton* saveBtn = new QPushButton("Save");
		connect(saveBtn, &QPushButton::clicked, this, [this, fileData]() {
			fileData->writeIntoFile();
			fileData->resetData();
			activeChanges.erase(fileData);
			updateBufferWidgets();
			});
		footerLayout->addWidget(saveBtn);
		groupLayout->addWidget(footer);


		for (uint16_t indexChanges : fileData->mActiveChanges) {
			DM::DataToken& token = fileData->mDataTokens[indexChanges];
			
			QListWidgetItem* item = new QListWidgetItem(list);

			QWidget* line = new QWidget(group);
			QHBoxLayout* lineLayout = new QHBoxLayout(line);
			lineLayout->addWidget(new QLabel(QString::fromUtf8(token.mKey.data(), static_cast<int>(token.mKey.size())) + " : ", line));
			lineLayout->addWidget(new QLabel(QString::fromStdString(token.getOriginName()),line));
			lineLayout->addWidget(new QLabel(QString::fromStdString(token.getCurrentName()),line));
			
			item->setSizeHint(line->sizeHint());
			list->setItemWidget(item, line);
		}

		mList->setItemWidget(item, group);
	}*/
	
}

void BufferTab::createSingleBufferWidget(QGroupBox* group, QVBoxLayout* groupLayout)
{

}

CachedWidget BufferTab::createWidgetForFileData(DM::iFileDataBase* fileData)
{
	QListWidgetItem* item = new QListWidgetItem(mList);
	item->setSizeHint(QSize(0, 250));

	auto [ptr, len] = fileData->getFileName();
	QGroupBox* group = new QGroupBox(QString::fromUtf8(ptr, len));
	group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	QVBoxLayout* groupLayout = new QVBoxLayout(group);
	groupLayout->setContentsMargins(4, 4, 4, 4);
	groupLayout->setSpacing(2);

	// Use QTableWidget instead of QListWidget with custom widgets
	QTableWidget* table = new QTableWidget(group);
	table->setColumnCount(3);
	table->setHorizontalHeaderLabels({ "Field", "Original Value", "Modified Value" });
	table->horizontalHeader()->setStretchLastSection(true);
	table->verticalHeader()->setVisible(false);
	table->setSelectionMode(QAbstractItemView::NoSelection);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);

	// CRITICAL performance settings for QTableWidget
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	table->verticalHeader()->setDefaultSectionSize(24);  // Fixed row height
	table->setShowGrid(true);  // or false, depending on your preference

	groupLayout->addWidget(table);

	// Footer buttons
	QWidget* footer = new QWidget(group);
	QHBoxLayout* footerLayout = new QHBoxLayout(footer);
	footerLayout->setContentsMargins(2, 2, 2, 2);

	QPushButton* cancelBtn = new QPushButton("Cancel");
	connect(cancelBtn, &QPushButton::clicked, this, [this, fileData]() {
		fileData->clearActiveChangedData();
		activeChanges.erase(fileData);
		updateBufferWidgets();
		});
	footerLayout->addWidget(cancelBtn);

	QPushButton* saveBtn = new QPushButton("Save");
	connect(saveBtn, &QPushButton::clicked, this, [this, fileData]() {
		fileData->writeIntoFile();
		fileData->resetData();
		activeChanges.erase(fileData);
		updateBufferWidgets();
		});
	footerLayout->addWidget(saveBtn);
	groupLayout->addWidget(footer);

	mList->setItemWidget(item, group);

	CachedWidget cached = { item, group, table, {} };
	populateTable(fileData, cached);

	return cached;
}

void BufferTab::updateCachedWidgetSmart(DM::iFileDataBase* fileData, CachedWidget& cached)
{
	std::unordered_set<uint16_t> currentIndices(
		fileData->mActiveChanges.begin(),
		fileData->mActiveChanges.end()
	);

	if (currentIndices == cached.cachedIndices) {
		return;  // No changes needed
	}

	cached.table->setUpdatesEnabled(false);
	populateTable(fileData, cached);
	cached.table->setUpdatesEnabled(true);
}

void BufferTab::populateTable(DM::iFileDataBase* fileData, CachedWidget& cached)
{
	{
		QTableWidget* table = cached.table;

		// Clear and resize
		table->setRowCount(0);
		table->setRowCount(fileData->mActiveChanges.size());
		cached.cachedIndices.clear();

		int row = 0;
		for (uint16_t indexChanges : fileData->mActiveChanges) {
			DM::DataToken& token = fileData->mDataTokens[indexChanges];

			// Use QTableWidgetItem instead of custom widgets - MUCH faster
			QTableWidgetItem* fieldItem = new QTableWidgetItem(
				QString::fromUtf8(token.mKey.data(), static_cast<int>(token.mKey.size()))
			);
			QTableWidgetItem* originItem = new QTableWidgetItem(
				QString::fromStdString(token.getOriginName())
			);
			QTableWidgetItem* currentItem = new QTableWidgetItem(
				QString::fromStdString(token.getCurrentName())
			);

			// Set items as non-editable
			fieldItem->setFlags(fieldItem->flags() & ~Qt::ItemIsEditable);
			originItem->setFlags(originItem->flags() & ~Qt::ItemIsEditable);
			currentItem->setFlags(currentItem->flags() & ~Qt::ItemIsEditable);

			table->setItem(row, 0, fieldItem);
			table->setItem(row, 1, originItem);
			table->setItem(row, 2, currentItem);

			cached.cachedIndices.insert(indexChanges);
			row++;
		}
	}
}


