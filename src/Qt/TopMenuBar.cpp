#include "TopMenuBar.h"

void TopMenuBar::setupMenus()
{
    QMenu* fileMenu = this->addMenu("File");
    QMenu* viewMenu = this->addMenu("View/MapMode");

    QAction* newAction = fileMenu->addAction("New");
    newAction->setEnabled(false);
    QAction* openAction = fileMenu->addAction("Open");
    openAction->setEnabled(false);
    QAction* exitAction = fileMenu->addAction("Exit");

    QAction* allProvinces = viewMenu->addAction("Provinces");
    QAction* allCountries = viewMenu->addAction("Countries");
    allCountries->setEnabled(false);
    QAction* provView = viewMenu->addAction("Search Country");
    //provView->setEnabled(false);
    
    viewMenu->addSeparator();

    QMenu* geoSMenu = viewMenu->addMenu("Geography");

    QAction* allAreas = geoSMenu->addAction("All Areas");
    QAction* allRegions = geoSMenu->addAction("All Regions");
    QAction* allSuperRegions = geoSMenu->addAction("All Super-Regions");
    QAction* allContinents = geoSMenu->addAction("All Continents");
    QAction* allColonial = geoSMenu->addAction("All Colonial Regions");
    allColonial->setEnabled(false);
    QAction* searchProv = viewMenu->addAction("Search Province");
    searchProv->setEnabled(false);
    QAction* searchArea = viewMenu->addAction("Search Area");
    searchArea->setEnabled(false);
    QAction* searchRegion = viewMenu->addAction("Search Region");
    searchRegion->setEnabled(false);
    QAction* searchSRegion = viewMenu->addAction("Search Super-Region");
    searchSRegion->setEnabled(false);
    QAction* searchContinent = viewMenu->addAction("Search Continent");
    searchContinent->setEnabled(false);
    viewMenu->addSeparator();

    QAction* allReligions = viewMenu->addAction("Religions");
    allReligions->setEnabled(false);
    QAction* allCultures = viewMenu->addAction("Cultures");
    allCultures->setEnabled(false);
    QAction* allCultureGroups = viewMenu->addAction("Culture-Groups");
    allCultureGroups->setEnabled(false);
    QAction* searchReligion = viewMenu->addAction("Search Religion");
    searchReligion->setEnabled(false);
    QAction* searchCulture = viewMenu->addAction("Search Culture");
    searchCulture->setEnabled(false);
    QAction* searchCultureGroup = viewMenu->addAction("Search Culture-Group");
    searchCultureGroup->setEnabled(false);
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
