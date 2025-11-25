#pragma once
#ifndef CULT_REL_BOX_H
#define CULT_REL_BOX_H
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include <QtCore\qstring.h>
#include <QtCore\qoverload.h>
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
    void loadProvInfo(Eu4::Province& province);
    void onCultureChanged(const QString& text);
protected:

private:
    QComboBox* cultCBox;
    QComboBox* relCBox;
    Eu4::Province* currentProvince = nullptr;
};

#endif // CULT_REL_BOX_H
