#include "MainApp.h"


MainApp::MainApp(const QApplication& app, FilePathHandler*& filePathHandler,QWidget* parent)
    : QMainWindow(parent)
{
    InitData(filePathHandler);

    TopMenuBar* menuBar = new TopMenuBar(app, this);
    menuBar->setupMenus();
    setMenuBar(menuBar);
    
    // when remaking the class and methods adjust with specific GameData right now they all work with Eu4::Data
    auto data = dynamic_cast<Eu4::Data*>(gameData);

    InformationGUI* rightMainArea = new InformationGUI(data->mGeoPolData, data->mCultRelData, data->mCountryData, data->mTGData);
    rightMainArea->loadWidgets();
    rightMainArea->initialiseWidgetsInfo();

    QWidget* centralWidget = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    ImageView* imageView = new ImageView(PROVINCE_MAP_FILE, data->mGeoPolData, *rightMainArea);
    mainLayout->addWidget(imageView, 2);
    mainLayout->addWidget(rightMainArea, 1);
    connect(menuBar, &TopMenuBar::changeView, imageView, &ImageView::changeView);

    setCentralWidget(centralWidget);
    setWindowTitle("Polybius ModTools");
    setMinimumSize(600, 400);
    resize(1920, 1080);
    show();
}

MainApp::~MainApp()
{
    delete gameData;
}

void MainApp::LoadWidgets()
{

}

void MainApp::InitWidgets()
{

}

void MainApp::InitData(FilePathHandler*& filePathHandler)
{

    if (filePathHandler->mGame == GAMES[0]) {
        gameData = new Eu4::Data(filePathHandler);
    }
    else {
        throw std::runtime_error("Wrong Game type");
    }
}
