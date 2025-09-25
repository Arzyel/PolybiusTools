#pragma once
#ifndef QT_UTILITIES_H
#define QT_UTILITIES_H
#include <QtCore\qobject.h>
#include <QtWidgets\qboxlayout.h>
#include <QtCore\qdebug.h>
#include <QtWidgets\qwidget.h>

inline void destroyChildren(QObject* parent) {
    if (!parent) return;

    // Make a copy of the list to avoid issues while deleting
    auto childrenCopy = parent->children();
    for (QObject* child : childrenCopy) {
        // Recursively destroy grandchildren first
        destroyChildren(child);

        // Schedule child for safe deletion
        child->deleteLater();
    }
}


inline void addToBoxLayout(QWidget* parent, QWidget* widget, int stretch = 0) {
    if (!parent || !widget) return;

    // Try to cast the parent layout to QBoxLayout
    QBoxLayout* layout = qobject_cast<QBoxLayout*>(parent->layout());
    if (!layout) {
        qWarning() << "Parent does not have a QBoxLayout!";
        return;
    }

    layout->addWidget(widget, stretch);
}
#endif // !QT_UTILITIES_H