#pragma once
#ifndef CULT_REL_BOX_H
#define CULT_REL_BOX_H
#include <functional>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>
#include <QtCore\qstring.h>
#include <QtCore\qoverload.h>
#include <cstdint>
#include <string_view>
#include "CultRelContainer.h"
#include "Eu4GeoPolUnits.h"


class CultRelBox : public QGroupBox{
    Q_OBJECT
public:
    CultRelBox() = delete;
    ~CultRelBox() = default;
    CultRelBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
    void makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable);
public slots:
    void initializeData(const std::unordered_map<std::string, sCulture>& cultureData,
        const std::unordered_map<std::string, sReligion>& religionData);
    void initializeData2(const std::vector<std::string_view>& allCultures, const std::vector<std::string_view>& allReligions);
    void loadProvInfo(Eu4::Province& province);
protected:

private:
    QComboBox* cultCBox;
    QComboBox* relCBox;
};

#endif // CULT_REL_BOX_H
