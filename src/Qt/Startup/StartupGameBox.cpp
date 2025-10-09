#include "StartupGameBox.h"

StartupGameBox::StartupGameBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QWidget* parent)
	:QWidget(parent), refContinueBtn(continueBtn)
{
    loadWidgets(filePathHandler);
}


void StartupGameBox::loadWidgets(FilePathHandler*& filePathHandler) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);

    QHBoxLayout* gameTypeLayout = new QHBoxLayout;
    QLabel* gameName = new QLabel("Game : ", this);
    gameName->setMinimumWidth(100);
    mGameType = new QComboBox(this);
    mGameType->setMinimumWidth(0);
    mGameType->setMaximumWidth(200);
    for (auto& game : GAMES) {
        mGameType->addItem(game);
    }
    gameName->setBuddy(mGameType);
    gameTypeLayout->addWidget(gameName);
    gameTypeLayout->addWidget(mGameType);
    gameTypeLayout->setAlignment(Qt::AlignLeft);


    QHBoxLayout* browseLayout = new QHBoxLayout;
    QLabel* gameFolderLabel = new QLabel("Game Folder : ");
    gameFolderLabel->setMinimumWidth(100);
    mGameFolderEdit = new QLineEdit;
    QPushButton* browseBtn = new QPushButton("Browse...");

    browseLayout->addWidget(gameFolderLabel);
    browseLayout->addWidget(mGameFolderEdit);
    browseLayout->addWidget(browseBtn);


    QHBoxLayout* expBrowseLayout = new QHBoxLayout;
    QLabel* exportFolderLabel = new QLabel("Export Folder : ");
    exportFolderLabel->setMinimumWidth(100);
    mExportFolderEdit = new QLineEdit;
    QPushButton* expBrowseBtn = new QPushButton("Browse...");

    expBrowseLayout->addWidget(exportFolderLabel);
    expBrowseLayout->addWidget(mExportFolderEdit);
    expBrowseLayout->addWidget(expBrowseBtn);



    QHBoxLayout* testLayout = new QHBoxLayout;
    QPushButton* checkBtn = new QPushButton("Check Paths");
    checkBtn->setMinimumWidth(100);
    checkBtn->setMaximumWidth(100);
    mStatusErrors = new QLabel(this);
    mStatusErrors->setWordWrap(true);
    mStatusErrors->setMinimumWidth(500);
    mStatusIndicator = new QLabel(this);
    mStatusIndicator->setMaximumWidth(50);
    mStatusIndicator->setMinimumWidth(50);

    testLayout->addWidget(checkBtn);
    testLayout->addWidget(mStatusIndicator);
    testLayout->addWidget(mStatusErrors);
    testLayout->setAlignment(Qt::AlignLeft);

    mainLayout->addLayout(gameTypeLayout);
    mainLayout->addLayout(browseLayout);
    mainLayout->addLayout(expBrowseLayout);
    mainLayout->addLayout(testLayout);



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


    connect(checkBtn, &QPushButton::clicked, this, [this, &filePathHandler] {
        GameFolders gameFolders;
        gameFolders.nameIndex = mGameType->currentIndex();
        gameFolders.gameFolder = mGameFolderEdit->text().toStdString();
        gameFolders.exportFolder = mExportFolderEdit->text().toStdString();

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
    std::cout << "Current directory: " << fs::current_path() << std::endl;

    // Check each level
    std::cout << "Exists '..': " << fs::exists("..") << std::endl;
    std::cout << "Exists '../..': " << fs::exists("../..") << std::endl;
    std::cout << "Exists 'init': " << fs::exists("init") << std::endl;
    std::cout << "Exists 'init/startup.ini': " << fs::exists("init/startup.ini") << std::endl;

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
            // Write updated line
            outFile << std::to_string(gameFolders.nameIndex) << ";"
                << gameFolders.gameFolder << ";"
                << gameFolders.exportFolder << "\n";
            linePresent = true;
        }
        else {
            // Write original line
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

    // Replace original file with temporary file
    if (std::remove(relative_path::STARTUP) != 0) {
        throw std::runtime_error("Error replacing original file");
    }
    if (std::rename(tempFilename.c_str(), relative_path::STARTUP) != 0) {
        throw std::runtime_error("Error renaming temporary file");
    }
}
GameFolders StartupGameBox::getGameFoldersData()
{
    GameFolders gameFolders;



    return gameFolders;
}
