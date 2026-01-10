#include "StartupModBox.h"

StartupModBox::StartupModBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QCheckBox* enableModFolderCheck, QWidget* parent)
	:QWidget(parent)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	this->setLayout(mainLayout);

	QLabel* label = new QLabel("The UI of this tab is in early developpement and subject to change without notice", this);
	mainLayout->addWidget(label);


	box = new QComboBox(this);
	mainLayout->addWidget(box);

	//connect(continueBtn, &QPushButton::clicked, this,
	//	[this, &filePathHandler]() {
	//		std::cout << filePathHandler->getModPath(box->currentIndex()) << std::endl;
	//	});


	connect(box, &QComboBox::currentIndexChanged, this,
		[this, &filePathHandler, enableModFolderCheck](int index) {
			if (enableModFolderCheck->isChecked()) {
				std::cout << filePathHandler->getModPath(index) << std::endl;
				filePathHandler->setActiveMod(index);
			}
		});

}

void StartupModBox::loadWidgets(const std::vector<ModFile>& modFiles)
{
	box->clear();
	for (const auto& modfile : modFiles) {
		box->addItem(QString::fromStdString(modfile.name));
	}
}
