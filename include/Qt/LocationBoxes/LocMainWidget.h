#pragma once
#ifndef PROVINCE_INFO_BOX_H
#define PROVINCE_INFO_BOX_H
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include "Eu4GeoPolData.h"
#include "Eu4GeoPolUnits.h"
#include "LocInfoBoxes.h"

class LocMainWidget : public QWidget {
	Q_OBJECT
public:
	LocMainWidget(QWidget* parent = nullptr);
	void loadWidgets();
public slots:
	void loadProvInfo(Eu4::Province& province);
protected:
private:
};


#endif // !PROVINCE_INFO_BOX_H
