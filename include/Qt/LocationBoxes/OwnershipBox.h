#pragma once
#ifndef OWNERSHIP_BOX_H
#define OWNERSHIP_BOX_H
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include <unordered_map>
#include <string>
#include <functional>
#include "Eu4GeoPolUnits.h"
#include "CultRelContainer.h"

class OwnershipBox : public QGroupBox{
    Q_OBJECT
public:
    OwnershipBox() = delete;
    ~OwnershipBox() = default;
    OwnershipBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
    void makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable);
public slots:
    void initializeData(const std::unordered_map<std::string, std::string>& data);
    void loadProvInfo(Eu4::Province& province);
protected:

private:
    QComboBox* ownerBox;
    QComboBox* controllerBox;
};

#endif // OWNERSHIP_BOX_H
