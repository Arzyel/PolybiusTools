#pragma once
#ifndef LOC_INFO_BOXES_H
#define LOC_INFO_BOXES_H
#include <QtWidgets\qgroupbox.h>
#include <QtCore\qstring.h>
#include <QtWidgets\qboxlayout.h>
#include <QtGui\qevent.h>
#include "BuildingsBox.h"
#include "CoresBox.h"
#include "CultRelBox.h"
#include "DevBox.h"
#include "OwnershipBox.h"
#include "TradeBox.h"
#include "FlowLayout.h"
#include "OwnershipBox.h"
#include "CoresBox.h"

class LocInfoBoxes : public QGroupBox {
    Q_OBJECT
public:
    LocInfoBoxes() = delete;
    ~LocInfoBoxes() = default;
    LocInfoBoxes(const QString& title, QWidget* parent = nullptr);
    void loadWidgets();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QVector<QGroupBox*> innerBoxes;
    QGridLayout* gridLayout;
    int currentColumns = 0;
    const int minBoxWidth = 150;
};

#endif // LOC_INFO_BOXES_H
