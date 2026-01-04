#include "TopMenuBar.h"

void TopMenuBar::setupMenus()
{
    QMenu* fileMenu = this->addMenu("File");
    QMenu* viewMenu = this->addMenu("View/MapMode");

    QAction* newAction = fileMenu->addAction("New");
    QAction* openAction = fileMenu->addAction("Open");
    QAction* exitAction = fileMenu->addAction("Exit");

    QAction* allProvinces = viewMenu->addAction("Provinces");
    QAction* allCountries = viewMenu->addAction("Countries");
    QAction* provView = viewMenu->addAction("Search Country");
    viewMenu->addSeparator();

    QMenu* geoSMenu = viewMenu->addMenu("Geography");

    QAction* allAreas = geoSMenu->addAction("All Areas");
    QAction* allRegions = geoSMenu->addAction("All Regions");
    QAction* allSuperRegions = geoSMenu->addAction("All Super-Regions");
    QAction* allContinents = geoSMenu->addAction("All Continents");
    QAction* allColonial = geoSMenu->addAction("All Colonial Regions");
    QAction* searchProv = viewMenu->addAction("Search Province");
    QAction* searchArea = viewMenu->addAction("Search Area");
    QAction* searchRegion = viewMenu->addAction("Search Region");
    QAction* searchSRegion = viewMenu->addAction("Search Super-Region");
    QAction* searchContinent = viewMenu->addAction("Search Continent");
    viewMenu->addSeparator();

    QAction* allReligions = viewMenu->addAction("Religions");
    QAction* allCultures = viewMenu->addAction("Cultures");
    QAction* allCultureGroups = viewMenu->addAction("Culture-Groups");
    QAction* searchReligion = viewMenu->addAction("Search Religion");
    QAction* searchCulture = viewMenu->addAction("Search Culture");
    QAction* searchCultureGroup = viewMenu->addAction("Search Culture-Group");
    viewMenu->addSeparator();

    connect(allProvinces, &QAction::triggered, this, [this] {
        emit changeView(0);
        });
    connect(allAreas, &QAction::triggered, this, [this] {
        emit changeView(1);
        });
    connect(allRegions, &QAction::triggered, this, [this] {
        emit changeView(2);
        });
    connect(allSuperRegions, &QAction::triggered, this, [this] {
        emit changeView(3);
        });
    connect(allContinents, &QAction::triggered, this, [this] {
        emit changeView(4);
        });

    QObject::connect(exitAction, &QAction::triggered, &mApp, &QApplication::quit);

}
