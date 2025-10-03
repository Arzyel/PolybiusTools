#pragma once
#ifndef OWNERSHIP_BOX_H
#define OWNERSHIP_BOX_H
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include "CultRelContainer.h"
#include "Location.h"


class OwnershipBox : public QGroupBox{
public:
    OwnershipBox() = delete;
    ~OwnershipBox() = default;
    OwnershipBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
public slots:
    void initializeData();
    void loadProvInfo(const Location& location);
protected:

private:
    QComboBox* ownerBox;
    QComboBox* controllerBox;
};

#endif // OWNERSHIP_BOX_H
