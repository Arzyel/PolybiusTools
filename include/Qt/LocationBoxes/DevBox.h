#pragma once
#ifndef DEV_BOX_H
#define DEV_BOX_H

#include <QtWidgets\qspinbox.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qpushbutton.h>
#include <random>
#include "Eu4GeoPolUnits.h"


//TODO refactor random out of the class
class DevBox : public QGroupBox {
    Q_OBJECT
public:
    DevBox() = delete;
    ~DevBox();
    DevBox(const QString& title, QWidget* parent = nullptr);
    void loadWidgets();

public slots:
    void loadDevInfo(const Eu4::Province& province);
protected:

private:
    QSpinBox* taxSpinBox;
    QSpinBox* prodSpinBox;
    QSpinBox* manSpinBox;
    std::mt19937 gen;
    std::uniform_int_distribution<> distLow;
    std::uniform_int_distribution<> distMid;
    std::uniform_int_distribution<> distHigh;
};

#endif // DEV_BOX_H
