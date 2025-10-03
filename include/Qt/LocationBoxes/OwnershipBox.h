#pragma once
#ifndef OWNERSHIP_BOX_H
#define OWNERSHIP_BOX_H
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include "CultRelContainer.h"
#include "Location.h"
#include <unordered_map>
#include <string>

class OwnershipBox : public QGroupBox{
    Q_OBJECT
public:
    OwnershipBox() = delete;
    ~OwnershipBox() = default;
    OwnershipBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
public slots:
    void initializeData(const std::unordered_map<std::string, std::string>& data);
protected:

private:
    QComboBox* ownerBox;
    QComboBox* controllerBox;
};

#endif // OWNERSHIP_BOX_H
