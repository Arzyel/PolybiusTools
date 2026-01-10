#pragma once
#ifndef STARTUP_MOD_BOX_H
#define STARTUP_MOD_BOX_H
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets/qcheckbox.h>
#include "FilePathHandler.h"

class StartupModBox : public QWidget{
    Q_OBJECT
public:
    StartupModBox() = delete;
    StartupModBox(FilePathHandler*& filePathHandler, QPushButton*& continueBtn, QCheckBox* enableModFolderCheck, QWidget* parent = nullptr);
    ~StartupModBox() = default;
    void loadWidgets(const std::vector<ModFile>& modFiles);

protected:

private:
    QComboBox* box = nullptr;
};

#endif // STARTUP_MOD_BOX_H
