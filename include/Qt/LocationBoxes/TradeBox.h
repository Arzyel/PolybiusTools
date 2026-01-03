#pragma once
#ifndef TRADE_BOX_H
#define TRADE_BOX_H
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

class TradeBox : public QGroupBox{
    Q_OBJECT
public:
    TradeBox() = delete;
    ~TradeBox() = default;
    TradeBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
    void makeConnections(std::function<void(uint16_t Eu4::Province::*, const std::string&)> callable);
public slots:
    void loadProvInfo(Eu4::Province& province);
    void initializeData(const std::vector<std::string_view>& tradeGoods);
protected:

private:
    QComboBox* mTGBox;
};

#endif // TRADE_BOX_H
