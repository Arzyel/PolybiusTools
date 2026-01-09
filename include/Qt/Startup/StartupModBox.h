#pragma once
#ifndef STARTUP_MOD_BOX_H
#define STARTUP_MOD_BOX_H
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qpushbutton.h>
#include "FilePathHandler.h"

class StartupModBox : public QWidget{
    Q_OBJECT
public:
    StartupModBox();
    ~StartupModBox() = default;
    void loadWidgets(std::vector<ModFile>& modFiles);

protected:

private:
    QComboBox* box = nullptr;
};

#endif // STARTUP_MOD_BOX_H
