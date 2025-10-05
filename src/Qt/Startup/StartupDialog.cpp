#include "StartupDialog.h"

StartupDialog::StartupDialog(QWidget* parent)
	:QDialog(parent)
{
	setupUI();
	setupConnections();
}

void StartupDialog::setupUI() {
	setWindowTitle("Polybius Launcher");
	setModal(true);
	setFixedSize(854, 480);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QTabWidget* tabWidget = new QTabWidget(this);
	QWidget* GameTab = new QWidget();
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

	// Create button layout
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	QPushButton* continueButton = new QPushButton("Continue");
	QPushButton* cancelButton = new QPushButton("Cancel");

	buttonLayout->addStretch(); // Push buttons to the right
	buttonLayout->addWidget(continueButton);
	buttonLayout->addWidget(cancelButton);

	// Add button layout to main layout
	mainLayout->addLayout(buttonLayout);

	// Store buttons for connections if needed
	mContinueButton = continueButton;
	mCancelButton = cancelButton;
	mContinueButton->setEnabled(false);

}


void StartupDialog::setupConnections() {
	connect(mContinueButton, &QPushButton::clicked, this, &StartupDialog::accept);
	connect(mCancelButton, &QPushButton::clicked, this, &StartupDialog::reject);
}