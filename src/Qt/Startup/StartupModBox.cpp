#include "StartupModBox.h"

StartupModBox::StartupModBox()
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	this->setLayout(mainLayout);

	box = new QComboBox(this);
	mainLayout->addWidget(box);
}

void StartupModBox::loadWidgets(std::vector<ModFile>& modFiles)
{
	for (const auto& modfile : modFiles) {
		box->addItem(QString::fromStdString(modfile.name));
	}
}
