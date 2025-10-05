#include <QtWidgets\qapplication.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtGui\qpainter.h>
#include <QtGui\qevent.h>
#include <QtGui\qaction.h>
#include <QtWidgets\qmainwindow.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qmenubar.h>
#include <QtWidgets\qscrollarea.h>
#include <QtWidgets\qscrollbar.h>
#include <QTWidgets\qtabwidget.h>
#include <QtWidgets\qlabel.h>
#include <QtGui\qimage.h>
#include <QtCore\qdebug.h>
#include <QtWidgets\qgraphicsview.h>
#include <QtWidgets\qgraphicswidget.h>
#include <QtWidgets\qstylefactory.h>

#include <chrono>

#include <iostream>
#include <string>
#include <InstantMenuStyle.h>
#include "ImageViewa.h"
#include "TopMenuBar.h"
#include "filenfolder_CONST.h"
#include "GeoPolContainers.h"
#include "InformationGUI.h"
#include "CultRelContainer.h"
#include "Eu4MainParser.h"
#include "CountryContainer.h"
#include "KEYS.h"
#include "Eu4Parser.h"
#include "StartupDialog.h"
#include "FilePathHandler.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    QApplication app(argc, argv);
    app.setStyle(new InstantMenuStyle(QStyleFactory::create("")));

    StartupDialog startupBox = StartupDialog(nullptr);


    if (startupBox.exec() != QDialog::Accepted) {
        return 0;
    }


    std::string culture = R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\cultures\00_cultures.txt)";
    std::string religion = R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\religions\00_religion.txt)";



    CultRelContainer cultRelContainer;
    cultRelContainer.loadCultureData(R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\cultures\00_cultures.txt)");
    cultRelContainer.loadReligionData(R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\religions\00_religion.txt)");

    CountryContainer countryContainer;
    countryContainer.initializeData();

    GeoPolContainers geoPolContainers;
    geoPolContainers.fillColorToID();
    geoPolContainers.initLocationData();


    QMainWindow window;
    
    TopMenuBar* menuBar = new TopMenuBar(app, &window);
    menuBar->setupMenus();
    window.setMenuBar(menuBar);

    

    InformationGUI* rightMainArea = new InformationGUI(geoPolContainers, cultRelContainer, countryContainer);
    rightMainArea->loadWidgets();
    rightMainArea->initialiseWidgetsInfo();

    // Main layout
    QWidget* centralWidget = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(new ImageView(PROVINCE_MAP_FILE,geoPolContainers,*rightMainArea), 2);      // left: ImageView
    mainLayout->addWidget(rightMainArea, 1);  // right: Tabs

    // Attach to QMainWindow
    window.setCentralWidget(centralWidget);
    window.setWindowTitle("Polybius ModTools");
    window.setMinimumSize(600, 400);
    window.resize(1200, 800); // wider for large image
    window.show();

    return app.exec();

   
}
