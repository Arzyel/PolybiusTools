#include "StartupModBox.h"

StartupModBox::StartupModBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QCheckBox* enableModFolderCheck, QWidget* parent)
	:QWidget(parent)
{
	QVBoxLayout* mainLayout = new QVBoxLayout;
	this->setLayout(mainLayout);

	QLabel* label = new QLabel("The UI of this tab is in early developpement and subject to change without notice.\n\n\n"
		"If the box is empty do these steps :\n"
		"\t1 - Go into the Game tab\n"
		"\t2 - Enable the chekbox labeled : Enable Loading Mods\n"
		"\t3 - Set the .mod folder\n"
		"\t4 - Press the Check Paths button\n"
		"\t5 - If the paths are correct go back into the Mod tab and you can now use the dropbox to select the mod you want to load", this);
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
