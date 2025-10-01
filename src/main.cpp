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

#include <iostream>
#include <string>
#include <InstantMenuStyle.h>
#include "ImageViewa.h"
#include "TopMenuBar.h"
#include "filenfolder_CONST.h"
#include "GeoPolContainers.h"
#include "InformationGUI.h"
//#include "CultRelContainer.h"
//#include "CultureParser.h"
#include "FileParser.h"


#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    FileParser parser;
    parser.parseFile(R"(E:\Workstation\Personal_Projects\Exercices\PolybiusTools\tests\test2.txt)");
    //parser.parseFile(R"(E:\Games\Steam\steamapps\common\Europa Universalis IV\common\cultures\00_cultures.txt)");

    GeoPolContainers geoPolContainers;
    geoPolContainers.fillColorToID();
    geoPolContainers.initLocationData();


    QApplication app(argc, argv);
    app.setStyle(new InstantMenuStyle(QStyleFactory::create("")));
    QMainWindow window;
    
    TopMenuBar* menuBar = new TopMenuBar(app, &window);
    menuBar->setupMenus();
    window.setMenuBar(menuBar);

    

    //QTabWidget* rightMainArea = new QTabWidget;
    //QWidget* tab1 = new QWidget;
    //QHBoxLayout* tab1Box = new QHBoxLayout(tab1);
    //rightMainArea->addTab(tab1, "Tab 1");
    InformationGUI* rightMainArea = new InformationGUI(geoPolContainers);
    rightMainArea->loadWidgets();

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
