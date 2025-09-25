#include "TopMenuBar.h"

void TopMenuBar::setupMenus()
{
    // Create menus
    QMenu* fileMenu = this->addMenu("File");
    QMenu* viewMenu = this->addMenu("View/MapMode");

    // Add actions to File menu
    QAction* newAction = fileMenu->addAction("New");
    QAction* openAction = fileMenu->addAction("Open");
    QAction* exitAction = fileMenu->addAction("Exit");



    // Add actions to View Menu
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


    // Connect action signals
    QObject::connect(exitAction, &QAction::triggered, &mApp, &QApplication::quit);

}
