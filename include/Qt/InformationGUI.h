#pragma once
#ifndef INFORMATION_GUI_H
#define INFORMATION_GUI_H
#include <QtWidgets/qtabwidget.h>
#include "GeoPolContainers.h"
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include "ProvinceInfoBox.h"
#include "QtUtils.h"

class InformationGUI : public QTabWidget {
public:
	InformationGUI(const GeoPolContainers& refGeoPolCont, QWidget* parent = nullptr);
	void loadWidgets();
	void loadProvInfo (const Location& location) const;
protected:
private:
	const GeoPolContainers& mRefGeoPolCont;
};


#endif // !INFORMATION_GUI_H
