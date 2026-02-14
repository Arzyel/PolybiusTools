#pragma once
#ifndef TOP_MENU_BAR
#define TOP_MENU_BAR
#include <QtWidgets\qmainwindow.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qmenubar.h>
#include <QtWidgets\qapplication.h>
#include <cstdint>
#include "SearchProv.h"


class TopMenuBar : public QMenuBar {
	Q_OBJECT
public:
	explicit TopMenuBar(const QApplication& app, QWidget* parent = nullptr)
		: QMenuBar(parent), mApp(app) {};
	void setupMenus();

signals:
    void changeView(uint8_t type);

protected:

private:
	const QApplication& mApp;
};

#endif // TOP_MENU_BAR
