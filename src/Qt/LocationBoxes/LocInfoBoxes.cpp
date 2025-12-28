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

	OwnershipBox* ownershipBox = new OwnershipBox("OwnerShip");
	ownershipBox->setMinimumWidth(minBoxWidth);
	ownershipBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	CoresBox* coresBox = new CoresBox("Cores");
	coresBox->setMinimumWidth(350);
	coresBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	flowLayout->addWidget(devBox);
	flowLayout->addWidget(cultRelBox);
	flowLayout->addWidget(ownershipBox);
	flowLayout->addWidget(coresBox);

}

void LocInfoBoxes::loadWidgets()
{
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
}
