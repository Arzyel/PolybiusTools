#include "MainApp.h"


MainApp::MainApp(const QApplication& app, FilePathHandler*& filePathHandler, QWidget* parent)
    : QMainWindow(parent)
{
    InitData(filePathHandler);

    TopMenuBar* menuBar = new TopMenuBar(app, this);
    menuBar->setupMenus();
    setMenuBar(menuBar);

    InformationGUI* rightMainArea = new InformationGUI(geoPolContainers, cultRelContainer, countryContainer);
    rightMainArea->loadWidgets();
    rightMainArea->initialiseWidgetsInfo();

    // Main layout
    QWidget* centralWidget = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(new ImageView(PROVINCE_MAP_FILE, geoPolContainers, *rightMainArea), 2);      // left: ImageView
    mainLayout->addWidget(rightMainArea, 1);  // right: Tabs

    // Attach to QMainWindow
    setCentralWidget(centralWidget);
    setWindowTitle("Polybius ModTools");
    setMinimumSize(600, 400);
    resize(1600, 800);
    show();
}

void MainApp::LoadWidgets()
{

}

void MainApp::InitWidgets()
{

}

void MainApp::InitData(FilePathHandler*& filePathHandler)
{
    std::vector<fs::path> ket1 = filePathHandler->getPathsFromFolderKey(relative_path::eu4::common::CULTURES_);
    cultRelContainer.loadCultureData(ket1);
    cultRelContainer.loadReligionData(R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\religions\00_religion.txt)");
    
    countryContainer.initializeData();

    geoPolContainers.fillColorToID();
    geoPolContainers.initLocationData();
}
