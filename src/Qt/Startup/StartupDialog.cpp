#include "StartupDialog.h"

StartupDialog::StartupDialog(FilePathHandler*& filePathHandler, QWidget* parent)
	:QDialog(parent)
{
	setupUI(filePathHandler);
	setupConnections();
}

void StartupDialog::setupUI(FilePathHandler*& filePathHandler) {
	setWindowTitle("Polybius Launcher");
	setModal(true);
	setFixedSize(854, 480);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QTabWidget* tabWidget = new QTabWidget(this);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	QPushButton* continueButton = new QPushButton("Continue");
	QPushButton* cancelButton = new QPushButton("Cancel");
	mContinueButton = continueButton;
	mCancelButton = cancelButton;
	mContinueButton->setEnabled(false);

	QCheckBox* enableModFolderCheck = new QCheckBox;
	StartupGameBox* GameTab = new StartupGameBox(filePathHandler,mContinueButton,enableModFolderCheck ,tabWidget);
	QWidget* ModTab = new StartupModBox(filePathHandler, mContinueButton, enableModFolderCheck, tabWidget);
	QWidget* LoadTab = new QWidget();
	QWidget* OptionsTab = new QWidget();

	GameTab->setStartupModBox(ModTab);

	
	tabWidget->addTab(GameTab, "Game");
	tabWidget->addTab(ModTab, "Mod");
	tabWidget->addTab(LoadTab, "Load Options");
	tabWidget->addTab(OptionsTab, "App Options");

	tabWidget->setTabEnabled(2, false);
	tabWidget->setTabEnabled(3, false);

	mainLayout->addWidget(tabWidget);

	setLayout(mainLayout);



	buttonLayout->addStretch();
	buttonLayout->addWidget(continueButton);
	buttonLayout->addWidget(cancelButton);

	mainLayout->addLayout(buttonLayout);


	connect(continueButton, &QPushButton::clicked, this,
		[this, &filePathHandler, enableModFolderCheck]() {
			if (enableModFolderCheck->isChecked()) {
				filePathHandler->updateAPWithModsAP();
			}
		});

}


void StartupDialog::setupConnections() {
	connect(mContinueButton, &QPushButton::clicked, this, &StartupDialog::accept);
	connect(mCancelButton, &QPushButton::clicked, this, &StartupDialog::reject);
}