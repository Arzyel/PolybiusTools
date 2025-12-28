#pragma once
#ifndef BUFFERED_CHANGES_MAIN_H
#define BUFFERED_CHANGES_MAIN_H
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qscrollbar.h>
#include "FileManager.h"


class BufferTab : public QWidget {
    Q_OBJECT
public:
    BufferTab(std::unordered_set<DM::iFileDataBase*>& setAChanges, QWidget* parent = nullptr);
    ~BufferTab() = default;
    void loadWidgets();
    void updateBufferWidgets();
protected:

private:
    std::unordered_set<DM::iFileDataBase*>& activeChanges;
    QListWidget* mList;
    void createSingleBufferWidget(QGroupBox* group, QVBoxLayout* groupLayout);
};

#endif // BUFFERED_CHANGES_MAIN_H
