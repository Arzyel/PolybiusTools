#pragma once
#ifndef STARTUP_DIALOG_H
#define STARTUP_DIALOG_H
#include <QtWidgets\qdialog.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qtabwidget.h>
#include "FilePathHandler.h"
#include "StartupGameBox.h"
#include "StartupLoadBox.h"
#include "StartupModBox.h"
#include "StartupOptionBox.h"


class StartupDialog : public QDialog{
    Q_OBJECT
public:
    StartupDialog() = delete;
    ~StartupDialog() = default;
    StartupDialog(FilePathHandler*& filePathHandler, QWidget* parent = nullptr);

protected:

private:
    void setupUI(FilePathHandler*& filePathHandler);
    void setupConnections();
    QPushButton* mContinueButton;
    QPushButton* mCancelButton;
};

#endif // STARTUP_DIALOG_H
