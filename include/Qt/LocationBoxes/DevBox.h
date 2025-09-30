#pragma once
#ifndef DEV_BOX_H
#define DEV_BOX_H

#include <QtWidgets\qspinbox.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include "Location.h"

class DevBox : public QGroupBox {
    Q_OBJECT
public:
    DevBox() = delete;
    ~DevBox();
    DevBox(const QString& title, QWidget* parent = nullptr);
    void loadWidgets();

public slots:
    void loadDevInfo(const Location& location);
protected:

private:
    QSpinBox* taxSpinBox;
    QSpinBox* prodSpinBox;
    QSpinBox* manSpinBox;
};

#endif // DEV_BOX_H
