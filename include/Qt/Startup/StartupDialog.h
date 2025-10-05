#pragma once
#ifndef STARTUP_DIALOG_H
#define STARTUP_DIALOG_H
#include <QtWidgets\qdialog.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qtabwidget.h>


class StartupDialog : public QDialog{
    Q_OBJECT
public:
    StartupDialog() = delete;
    ~StartupDialog() = default;
    StartupDialog(QWidget* parent = nullptr);

protected:

private:
    void setupUI();
    void setupConnections();
    QPushButton* mContinueButton;
    QPushButton* mCancelButton;
};

#endif // STARTUP_DIALOG_H
