#pragma once
#ifndef INFORMATION_GUI_H
#define INFORMATION_GUI_H
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include <QtCore\qobject.h>
#include <QtCore\qdebug.h>
#include "LocMainWidget.h"
#include "GeoPolContainers.h"
#include "QtUtils.h"

class InformationGUI : public QTabWidget {
	Q_OBJECT
public:
	InformationGUI(const GeoPolContainers& refGeoPolCont, QWidget* parent = nullptr);
	void loadWidgets();
	void loadProvInfo (const Location& location) const;
signals:
	void signalLoadInfo(const Location& location) const;
protected:
private:
	const GeoPolContainers& mRefGeoPolCont;
};


#endif // !INFORMATION_GUI_H
