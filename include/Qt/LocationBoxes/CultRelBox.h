#pragma once
#ifndef CULT_REL_BOX_H
#define CULT_REL_BOX_H
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include "CultRelContainer.h"
#include "Eu4GeoPolUnits.h"


class CultRelBox : public QGroupBox{
    Q_OBJECT
public:
    CultRelBox() = delete;
    ~CultRelBox() = default;
    CultRelBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
public slots:
    void initializeData(const std::unordered_map<std::string, sCulture>& cultureData,
        const std::unordered_map<std::string, sReligion>& religionData);
    void loadProvInfo(const Eu4::Province& province);
protected:

private:
    QComboBox* cultCBox;
    QComboBox* relCBox;
};

#endif // CULT_REL_BOX_H
