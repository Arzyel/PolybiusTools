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

	StartupGameBox* GameTab = new StartupGameBox(filePathHandler,mContinueButton, nullptr);
	QWidget* ModTab = new QWidget();
	QWidget* LoadTab = new QWidget();
	QWidget* OptionsTab = new QWidget();
	
	tabWidget->addTab(GameTab, "Game");
	tabWidget->addTab(ModTab, "Mod");
	tabWidget->addTab(LoadTab, "Load Options");
	tabWidget->addTab(OptionsTab, "App Options");

	tabWidget->setTabEnabled(1, false);
	tabWidget->setTabEnabled(2, false);

	mainLayout->addWidget(tabWidget);

	setLayout(mainLayout);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	QPushButton* continueButton = new QPushButton("Continue");
	QPushButton* cancelButton = new QPushButton("Cancel");

	buttonLayout->addStretch();
	buttonLayout->addWidget(continueButton);
	buttonLayout->addWidget(cancelButton);

	mainLayout->addLayout(buttonLayout);

	mContinueButton = continueButton;
	mCancelButton = cancelButton;
	mContinueButton->setEnabled(false);

}


void StartupDialog::setupConnections() {
	connect(mContinueButton, &QPushButton::clicked, this, &StartupDialog::accept);
	connect(mCancelButton, &QPushButton::clicked, this, &StartupDialog::reject);
}