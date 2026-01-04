#pragma once
#ifndef MAIN_APP_H
#define MAIN_APP_H
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qapplication.h>
#include "TopMenuBar.h"
#include "FilePathHandler.h"
#include "CultRelContainer.h"
#include "CountryContainer.h"
#include "Eu4GeoPolData.h"
#include "InformationGUI.h"
#include "ImageViewa.h"
#include "GameData.h"
#include "Data.h"
#include "KEYS.h"
#include "FileManager.h"

class MainApp : public QMainWindow{
    Q_OBJECT
public:
    MainApp(const QApplication& app, FilePathHandler*& filePathHandler,QWidget* parent = nullptr);
    ~MainApp();

protected:

private:
    GameData* gameData = nullptr;

    void LoadWidgets();
    void InitWidgets();
    void InitData(FilePathHandler*& filePathHandler);

};

#endif // MAIN_APP_H
