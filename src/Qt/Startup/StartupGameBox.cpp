#include "StartupGameBox.h"

StartupGameBox::StartupGameBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QWidget* parent)
	:QWidget(parent), refContinueBtn(continueBtn)
{
    loadWidgets(filePathHandler);
}


void StartupGameBox::loadWidgets(FilePathHandler*& filePathHandler) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    // ---- Form layout for labeled inputs ----
    QFormLayout* form = new QFormLayout;
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    form->setHorizontalSpacing(8);
    form->setVerticalSpacing(6);

    // Helper to wrap row layouts (Qt Designer parity)
    auto makeRowWidget = [&](QHBoxLayout* rowLayout) {
        QWidget* w = new QWidget(this);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(6);
        w->setLayout(rowLayout);
        return w;
        };

    // ---- Game selection ----
    mGameType = new QComboBox(this);
    mGameType->setMaximumWidth(200);

    for (const auto& game : GAMES)
        mGameType->addItem(game);

    QLabel* gameName = new QLabel("Game :", this);
    gameName->setBuddy(mGameType);

    form->addRow(gameName, mGameType);

    // ---- Game folder ----
    mGameFolderEdit = new QLineEdit(this);
    QPushButton* browseBtn = new QPushButton("Browse...", this);

    QHBoxLayout* gameFolderRowLayout = new QHBoxLayout;
    gameFolderRowLayout->addWidget(mGameFolderEdit);
    gameFolderRowLayout->addWidget(browseBtn);

    form->addRow("Game Folder :", makeRowWidget(gameFolderRowLayout));

    // ---- Export folder ----
    mExportFolderEdit = new QLineEdit(this);
    QPushButton* expBrowseBtn = new QPushButton("Browse...", this);

    QHBoxLayout* exportFolderRowLayout = new QHBoxLayout;
    exportFolderRowLayout->addWidget(mExportFolderEdit);
    exportFolderRowLayout->addWidget(expBrowseBtn);

    form->addRow("Export Folder :", makeRowWidget(exportFolderRowLayout));

    // ---- Enable mod loading ----
    QCheckBox* enableModFolderCheck = new QCheckBox(this);
    form->addRow("Enable Loading Mods :", enableModFolderCheck);

    // ---- Mod folder ----
    mModFolderEdit = new QLineEdit(this);
    QPushButton* modBrowseBtn = new QPushButton("Browse...", this);

    QHBoxLayout* modFolderRowLayout = new QHBoxLayout;
    modFolderRowLayout->addWidget(mModFolderEdit);
    modFolderRowLayout->addWidget(modBrowseBtn);

    QWidget* modFolderRowWidget = makeRowWidget(modFolderRowLayout);
    form->addRow(".mod Folder :", modFolderRowWidget);

    // Enable / disable only this row
    connect(enableModFolderCheck, &QCheckBox::toggled,
        modFolderRowWidget, &QWidget::setEnabled);

    // ---- Mod folder description ----
    QLabel* modFolderLabel = new QLabel(
        "Note: the .mod folder is usually located at\n"
        "C:\\Users\\user\\Documents\\Paradox Interactive\\Europa Universalis IV\\mod",
        this
    );
    modFolderLabel->setWordWrap(true);

    // Field-column only (no extra spacing)
    form->addRow(QString(), modFolderLabel);

    // ---- Add form to main layout ----
    mainLayout->addLayout(form);

    // ---- Bottom action / status row ----
    QHBoxLayout* testLayout = new QHBoxLayout;
    testLayout->setSpacing(8);
    testLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* checkBtn = new QPushButton("Check Paths", this);
    checkBtn->setFixedWidth(100);

    mStatusIndicator = new QLabel(this);
    mStatusIndicator->setFixedWidth(50);

    mStatusErrors = new QLabel(this);
    mStatusErrors->setWordWrap(true);

    testLayout->addWidget(checkBtn);
    testLayout->addWidget(mStatusIndicator);
    testLayout->addWidget(mStatusErrors);
    testLayout->addStretch();

    mainLayout->addLayout(testLayout);





    getStartupPaths();
    connect(mGameType, QOverload<int>::of(&QComboBox::activated),
        this, &StartupGameBox::onGameTypeActivated);
    emit mGameType->activated(0);

    connect(browseBtn, &QPushButton::clicked, this, [this]() {
        QString folder = QFileDialog::getExistingDirectory(this, "Select Folder");
        if (!folder.isEmpty()) {
            mGameFolderEdit->setText(folder);
        }
        });

    connect(expBrowseBtn, &QPushButton::clicked, this, [this]() {
        QString folder = QFileDialog::getExistingDirectory(this, "Select Folder");
        if (!folder.isEmpty()) {
            mExportFolderEdit->setText(folder);
        }
        });
    connect(modBrowseBtn, &QPushButton::clicked, this, [this]() {
        QString folder = QFileDialog::getExistingDirectory(this, "Select Folder");
        if (!folder.isEmpty()) {
            mModFolderEdit->setText(folder);
        }
        });


    connect(checkBtn, &QPushButton::clicked, this, [this, &filePathHandler, enableModFolderCheck] {
        GameFolders gameFolders;
        gameFolders.nameIndex = mGameType->currentIndex();
        gameFolders.gameFolder = mGameFolderEdit->text().toStdString();
        gameFolders.exportFolder = mExportFolderEdit->text().toStdString();
        delete filePathHandler;
        filePathHandler = nullptr;

        try {
            mStatusErrors->setText("");
            filePathHandler = FilePathHandlerFactory::createFPH(GAMES[gameFolders.nameIndex], gameFolders.gameFolder,
                gameFolders.exportFolder);
            filePathHandler->initAllPaths();
            mStatusIndicator->setText("✓");
            mStatusIndicator->setStyleSheet("color: green; font-size: 30px;");
            mStatusErrors->setText("All folders and minimum file requirement met. Click Continue to open the program or go setup mods.");
            refContinueBtn->setEnabled(true);
            saveStartupPaths(gameFolders);


            // Mod folder loading
            if (enableModFolderCheck->isChecked()) {
                std::cout << "box cehcked" << std::endl;
            }
        }
        catch (const std::exception& e) {
            delete filePathHandler;
            filePathHandler = nullptr;
            refContinueBtn->setEnabled(false);
            mStatusErrors->setText(e.what());
            mStatusIndicator->setText("✗");
            mStatusIndicator->setStyleSheet("color: red; font-size: 30px;");
        }
        catch (...) {
            std::cerr << "FilePathHandlerFactory CRITICAL: Unknown exception" << std::endl;
        }
        });

}

void StartupGameBox::saveStartupPaths(GameFolders& gameFolders)
{
    std::ifstream inFile(relative_path::STARTUP);
    if (!inFile.is_open()) {
        throw std::runtime_error("Error opening startup data file for reading");
    }

    std::string tempFilename = std::string(relative_path::STARTUP) + ".tmp";
    std::ofstream outFile(tempFilename);
    if (!outFile.is_open()) {
        inFile.close();
        throw std::runtime_error("Error creating temporary file for writing");
    }

    std::string line;
    bool linePresent = false;
    while (std::getline(inFile, line)) {
        auto delimiter = line.find(';');
        if (delimiter != std::string::npos &&
            std::stoi(line.substr(0, delimiter)) == gameFolders.nameIndex) {
            outFile << std::to_string(gameFolders.nameIndex) << ";"
                << gameFolders.gameFolder << ";"
                << gameFolders.exportFolder << "\n";
            linePresent = true;
        }
        else {
            outFile << line << "\n";
        }
    }
    if (!linePresent) {
        outFile << std::to_string(gameFolders.nameIndex) << ";"
            << gameFolders.gameFolder << ";"
            << gameFolders.exportFolder << "\n";
    }
    inFile.close();
    outFile.close();

    if (std::remove(relative_path::STARTUP) != 0) {
        throw std::runtime_error("Error replacing original file");
    }
    if (std::rename(tempFilename.c_str(), relative_path::STARTUP) != 0) {
        throw std::runtime_error("Error renaming temporary file");
    }
}
int StartupGameBox::getStartupPaths()
{
    std::ifstream startFile(relative_path::STARTUP);
    if (!startFile.is_open())
    {
        return 1;
    }

    for (int i = 0; i < GAMES.size(); ++i) {
        mSavedPaths[i];
    }


    std::string line;
    while (std::getline(startFile, line))
    {
        if (line.empty()) continue;
        GameFolders gameF;
        const char* start = line.data();
        const char* end = start + line.size();

        const char* ptr = start;


        std::string tempChars = "";
        while (*ptr != ';') {
            tempChars.push_back(*ptr);
            ++ptr;
        }
        ++ptr;
        gameF.nameIndex = std::stoi(tempChars);

        while (*ptr != ';') {
            gameF.gameFolder.push_back(*ptr);
            ++ptr;
        }
        ++ptr;
        while (ptr < end) {
            gameF.exportFolder.push_back(*ptr);
            ++ptr;
        }

        mSavedPaths[gameF.nameIndex] = gameF;
    }
    return 0;
}

void StartupGameBox::onGameTypeActivated(int index)
{
    if (mSavedPaths.contains(index)) {
        GameFolders temp = mSavedPaths.at(index);
        mGameFolderEdit->setText(QString::fromStdString(temp.gameFolder));
        mExportFolderEdit->setText(QString::fromStdString(temp.exportFolder));
    }

}



