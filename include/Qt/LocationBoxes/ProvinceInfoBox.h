#pragma once
#ifndef PROVINCE_INFO_BOX_H
#define PROVINCE_INFO_BOX_H
#include <QtWidgets/qtabwidget.h>
#include "GeoPolContainers.h"
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include "Location.h"

class ProvinceInfoBox : public QWidget {
	Q_OBJECT
public:
	ProvinceInfoBox(QWidget* parent = nullptr);
	void loadWidgets();

public slots:
	void loadProvInfo(const Location& location);
protected:
private:
};


#endif // !PROVINCE_INFO_BOX_H
