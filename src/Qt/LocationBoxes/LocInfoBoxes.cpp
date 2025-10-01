#include "LocInfoBoxes.h"



LocInfoBoxes::LocInfoBoxes(const QString& title, QWidget* parent)
	:QGroupBox(title, parent)
{
	//gridLayout = new QGridLayout(this);
	//gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
	//gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//this->setLayout(gridLayout);

	FlowLayout* flowLayout = new FlowLayout(this, 20, 10, 10);

	DevBox* devBox = new DevBox("Developpment");
	devBox->setMinimumWidth(minBoxWidth);
	devBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


	CultRelBox* cultRelBox = new CultRelBox("Culture and Religion");
	cultRelBox->setMinimumWidth(minBoxWidth);
	cultRelBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



	flowLayout->addWidget(devBox);
	flowLayout->addWidget(cultRelBox);

}

void LocInfoBoxes::loadWidgets()
{
	//QHBoxLayout* locInfoBoxLayout = new QHBoxLayout(this);
	//
	//DevBox* devBox = new DevBox("Developpment");
	//CultRelBox* cultRelBox = new CultRelBox("Culture and Religion");
	//locInfoBoxLayout->addWidget(devBox);
	//locInfoBoxLayout->addWidget(cultRelBox);

	
}

void LocInfoBoxes::resizeEvent(QResizeEvent* event)
{
	QGroupBox::resizeEvent(event);

	// Get the top-level window width
	int windowWidth = this->window()->width();

	// Decide the number of columns based on window width
	int columns = (windowWidth <= 1000) ? 1 : 2;

	if (columns == currentColumns)
		return;
	// Clear the layout first
	//for (QWidget* w : innerBoxes)
	//	gridLayout->removeWidget(w);

	//currentColumns = columns;

	//// Reposition boxes
	//for (int i = 0; i < innerBoxes.size(); ++i) {
	//	int row = i / columns;
	//	int col = i % columns;
	//	gridLayout->addWidget(innerBoxes[i], row, col);
	//}

	//// Force layout update immediately
	//gridLayout->update();
	//gridLayout->activate();
}
