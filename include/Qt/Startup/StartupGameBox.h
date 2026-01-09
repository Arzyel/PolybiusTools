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
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <vector>

#include "KEYS.h"
#include "FilePathHandler.h"
#include "StartupModBox.h"

struct GameFolders {
    int nameIndex;
    std::string gameFolder;
    std::string exportFolder;
    std::string modsFolder;
};

class StartupGameBox : public QWidget{
    Q_OBJECT
public:
    StartupGameBox() = delete;
    ~StartupGameBox() = default;
    StartupGameBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QWidget* parent = nullptr);
    void setStartupModBox(QWidget* modbox);

protected:

private:
    QComboBox* mGameType;
    QLineEdit* mGameFolderEdit;
    QLineEdit* mExportFolderEdit;
    QLineEdit* mModFolderEdit;
    QLabel* mStatusIndicator;
    QLabel* mStatusErrors;
    QPushButton*& refContinueBtn;
    QWidget* modFolderRowWidget;
    std::unordered_map<int, GameFolders> mSavedPaths;
    QWidget* startupModBox = nullptr;

    void loadWidgets(FilePathHandler*& filePathHandler);
    void saveStartupPaths(GameFolders& gameFolders);
    int getStartupPaths();
    void onGameTypeActivated(int index);
};

#endif // STARTUP_GAME_BOX_H
