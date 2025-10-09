#pragma once
#ifndef STARTUP_GAME_BOX_H
#define STARTUP_GAME_BOX_H

#include <QtWidgets\qwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qfiledialog.h>
#include <QtWidgets\qlineedit.h>
#include <string>
#include <iostream>
#include <fstream>

#include "KEYS.h"
#include "FilePathHandler.h"

struct GameFolders {
    int nameIndex;
    std::string gameFolder;
    std::string exportFolder;
};

class StartupGameBox : public QWidget{
    Q_OBJECT
public:
    StartupGameBox() = delete;
    ~StartupGameBox() = default;
    StartupGameBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QWidget* parent = nullptr);
    
    
public slots:
    GameFolders getGameFoldersData();

protected:

private:
    QComboBox* mGameType;
    QLineEdit* mGameFolderEdit;
    QLineEdit* mExportFolderEdit;
    QLabel* mStatusIndicator;
    QLabel* mStatusErrors;
    QPushButton*& refContinueBtn;
    std::unordered_map<int, GameFolders> mSavedPaths;

    void loadWidgets(FilePathHandler*& filePathHandler);
    void saveStartupPaths(GameFolders& gameFolders);
    int getStartupPaths();
    void onGameTypeActivated(int index);
};

#endif // STARTUP_GAME_BOX_H
