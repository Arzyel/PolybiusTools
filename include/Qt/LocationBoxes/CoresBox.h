#pragma once
#ifndef CORES_BOX_H
#define CORES_BOX_H

#include <unordered_map>
#include <string>
#include <functional>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets/qtablewidget.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qpushbutton.h>
#include "Eu4GeoPolUnits.h"


class CoresBox : public QGroupBox{
    Q_OBJECT
public:
    CoresBox() = delete;
    CoresBox(const QString& title, QWidget* parent = nullptr);
    ~CoresBox() = default;
    void loadWidget();
    void makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable);
public slots:
    void initializeData(const std::unordered_map<std::string, std::string>& data);
    void loadProvInfo(Eu4::Province& province);
protected:

private:
    QComboBox* box = nullptr;
    QTableWidget* table = nullptr;

    void addRow(const QString& tag);
};

#endif // CORES_BOX_H
