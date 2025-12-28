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

	connect(mList->verticalScrollBar(), &QScrollBar::valueChanged,
		this, &BufferTab::onScroll);

	// Install event filter for viewport resize events
	mList->viewport()->installEventFilter(this);

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

	// Step 1: Remove widgets no longer in activeChanges
	for (auto it = mWidgetCache.begin(); it != mWidgetCache.end(); ) {
		if (activeChanges.find(it->first) == activeChanges.end()) {
			delete it->second.item;
			it = mWidgetCache.erase(it);
		}
		else {
			++it;
		}
	}

	// Step 2: Create/update placeholder items for all active changes
	for (auto fileData : activeChanges) {
		auto it = mWidgetCache.find(fileData);

		if (it == mWidgetCache.end()) {
			// Create new placeholder
			QListWidgetItem* item = new QListWidgetItem(mList);
			item->setSizeHint(QSize(0, 250));
			item->setData(Qt::UserRole, QVariant::fromValue(fileData));

			CachedWidget cached;
			cached.item = item;
			cached.fileData = fileData;
			cached.isCreated = false;

			mWidgetCache[fileData] = cached;
		}
		else {
			// Update existing widget if it's created
			if (it->second.isCreated) {
				updateWidgetSmartly(fileData, it->second);
			}
		}
	}

	mList->setUpdatesEnabled(true);

	qDebug() << "updateBufferWidgets took:" << timer.elapsed() << "ms";

	// Step 3: Create widgets for visible items
	QTimer::singleShot(0, this, &BufferTab::updateVisibleWidgets);
}

void BufferTab::onScroll()
{
	updateVisibleWidgets();
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



void BufferTab::populateTable(DM::iFileDataBase* fileData, CachedWidget& cached)
{
	QTableWidget* table = cached.table;

	table->setUpdatesEnabled(false);
	table->setRowCount(0);
	table->setRowCount(fileData->mActiveChanges.size());
	cached.indexToRow.clear();
	cached.cachedValues.clear();

	int row = 0;
	for (uint16_t indexChanges : fileData->mActiveChanges) {
		DM::DataToken& token = fileData->mDataTokens[indexChanges];

		std::string originName = token.getOriginName();
		std::string currentName = token.getCurrentName();

		QTableWidgetItem* fieldItem = new QTableWidgetItem(
			QString::fromUtf8(token.mKey.data(), static_cast<int>(token.mKey.size()))
		);
		QTableWidgetItem* originItem = new QTableWidgetItem(
			QString::fromStdString(originName)
		);
		QTableWidgetItem* currentItem = new QTableWidgetItem(
			QString::fromStdString(currentName)
		);

		fieldItem->setFlags(fieldItem->flags() & ~Qt::ItemIsEditable);
		originItem->setFlags(originItem->flags() & ~Qt::ItemIsEditable);
		currentItem->setFlags(currentItem->flags() & ~Qt::ItemIsEditable);

		table->setItem(row, 0, fieldItem);
		table->setItem(row, 1, originItem);
		table->setItem(row, 2, currentItem);

		cached.indexToRow[indexChanges] = row;
		cached.cachedValues[indexChanges] = { originName, currentName };
		row++;
	}

	table->setUpdatesEnabled(true);
}



void BufferTab::updateWidgetSmartly(DM::iFileDataBase* fileData, CachedWidget& cached)
{
	// Build current indices set
	std::unordered_set<uint16_t> currentIndices(
		fileData->mActiveChanges.begin(),
		fileData->mActiveChanges.end()
	);

	std::unordered_set<uint16_t> cachedIndices;
	for (const auto& [idx, row] : cached.indexToRow) {
		cachedIndices.insert(idx);
	}

	// Check for structural changes (additions/removals)
	bool structuralChange = false;

	// Check if any indices were added or removed
	if (currentIndices.size() != cachedIndices.size()) {
		structuralChange = true;
	}
	else {
		for (uint16_t idx : currentIndices) {
			if (cachedIndices.find(idx) == cachedIndices.end()) {
				structuralChange = true;
				break;
			}
		}
	}

	if (structuralChange) {
		// Rows added/removed, need full rebuild
		cached.table->setUpdatesEnabled(false);
		populateTable(fileData, cached);
		cached.table->setUpdatesEnabled(true);
	}
	else {
		// Same rows, update only changed cells
		updateChangedCells(fileData, cached, currentIndices);
	}
}

void BufferTab::updateChangedCells(DM::iFileDataBase* fileData, CachedWidget& cached, const std::unordered_set<uint16_t>& currentIndices)
{
	cached.table->setUpdatesEnabled(false);

	for (uint16_t idx : currentIndices) {
		DM::DataToken& token = fileData->mDataTokens[idx];
		std::string originName = token.getOriginName();
		std::string currentName = token.getCurrentName();

		auto cachedIt = cached.cachedValues.find(idx);

		// Check if values changed
		bool originChanged = (cachedIt == cached.cachedValues.end() ||
			cachedIt->second.first != originName);
		bool currentChanged = (cachedIt == cached.cachedValues.end() ||
			cachedIt->second.second != currentName);

		if (originChanged || currentChanged) {
			int row = cached.indexToRow[idx];

			// Update only the changed cells
			if (originChanged) {
				QTableWidgetItem* originItem = cached.table->item(row, 1);
				if (originItem) {
					originItem->setText(QString::fromStdString(originName));
				}
			}

			if (currentChanged) {
				QTableWidgetItem* currentItem = cached.table->item(row, 2);
				if (currentItem) {
					currentItem->setText(QString::fromStdString(currentName));
				}
			}

			// Update cached values
			cached.cachedValues[idx] = { originName, currentName };
		}
	}

	cached.table->setUpdatesEnabled(true);
}

void BufferTab::createWidgetForItem(DM::iFileDataBase* fileData, CachedWidget& cached)
{
	if (cached.isCreated) return;

	auto [ptr, len] = fileData->getFileName();
	QGroupBox* group = new QGroupBox(QString::fromUtf8(ptr, len));
	group->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	QVBoxLayout* groupLayout = new QVBoxLayout(group);
	groupLayout->setContentsMargins(4, 4, 4, 4);
	groupLayout->setSpacing(2);

	// Table
	QTableWidget* table = new QTableWidget(group);
	table->setColumnCount(3);
	table->setHorizontalHeaderLabels({ "Field", "Original Value", "Modified Value" });
	table->horizontalHeader()->setStretchLastSection(true);
	table->verticalHeader()->setVisible(false);
	table->setSelectionMode(QAbstractItemView::NoSelection);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	table->verticalHeader()->setDefaultSectionSize(24);
	table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	groupLayout->addWidget(table);

	// Footer
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

	// Set the widget to the list item
	mList->setItemWidget(cached.item, group);

	// Update cached references
	cached.group = group;
	cached.table = table;
	cached.isCreated = true;

	// Populate the table
	populateTable(fileData, cached);
}

void BufferTab::destroyWidget(CachedWidget& cached)
{
	if (!cached.isCreated) return;

	// Remove widget from list item (but keep the item itself)
	mList->setItemWidget(cached.item, nullptr);

	// Delete the widget
	delete cached.group;

	// Clear references
	cached.group = nullptr;
	cached.table = nullptr;
	cached.isCreated = false;
	cached.indexToRow.clear();
	cached.cachedValues.clear();
}

void BufferTab::updateVisibleWidgets()
{
	QElapsedTimer timer;
	timer.start();

	QRect viewportRect = mList->viewport()->rect();
	int viewportTop = viewportRect.top();
	int viewportBottom = viewportRect.bottom();

	int created = 0;
	int destroyed = 0;

	for (int i = 0; i < mList->count(); ++i) {
		QListWidgetItem* item = mList->item(i);
		QRect itemRect = mList->visualItemRect(item);

		DM::iFileDataBase* fileData = item->data(Qt::UserRole).value<DM::iFileDataBase*>();
		auto it = mWidgetCache.find(fileData);

		if (it == mWidgetCache.end()) continue;

		int itemTop = itemRect.top();
		int itemBottom = itemRect.bottom();

		// Check if item should be visible (with preload buffer)
		bool shouldBeVisible = (itemBottom >= viewportTop - 500 &&
			itemTop <= viewportBottom + 500);

		// Check if item is far from viewport (for cleanup)
		bool isFarAway = (itemBottom < viewportTop - 2000 ||
			itemTop > viewportBottom + 2000);

		if (shouldBeVisible && !it->second.isCreated) {
			// Create widget
			createWidgetForItem(fileData, it->second);
			created++;
		}
		else if (isFarAway && it->second.isCreated) {
			// Destroy widget to save memory
			destroyWidget(it->second);
			destroyed++;
		}
	}

	if (created > 0 || destroyed > 0) {
		qDebug() << "Lazy loading: created" << created << "destroyed" << destroyed
			<< "took" << timer.elapsed() << "ms";
	}
}

bool BufferTab::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == mList->viewport() && event->type() == QEvent::Resize) {
		QTimer::singleShot(0, this, &BufferTab::updateVisibleWidgets);
	}
	return QWidget::eventFilter(obj, event);
}


