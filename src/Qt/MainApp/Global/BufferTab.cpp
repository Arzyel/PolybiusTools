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
	mList->clear();
	for (auto fileData : activeChanges) {
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
	}
}

void BufferTab::createSingleBufferWidget(QGroupBox* group, QVBoxLayout* groupLayout)
{

}

