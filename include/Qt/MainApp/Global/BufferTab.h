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
#include <QtCore/qelapsedtimer.h>
#include <QtWidgets/qtablewidget.h>
#include <QtWidgets/qheaderview.h>
#include <unordered_map>
#include <unordered_set>
#include "FileManager.h"

struct CachedWidget {
    QListWidgetItem* item;
    QGroupBox* group;
    QTableWidget* table;
    std::unordered_set<uint16_t> cachedIndices;
};
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
    std::unordered_map<DM::iFileDataBase*, CachedWidget> mWidgetCache;
    void createSingleBufferWidget(QGroupBox* group, QVBoxLayout* groupLayout);
    // Claude ai solution for performance
    CachedWidget createWidgetForFileData(DM::iFileDataBase* fileData);
    void updateCachedWidgetSmart(DM::iFileDataBase* fileData, CachedWidget& cached);
    void populateTable(DM::iFileDataBase* fileData, CachedWidget& cached);
   

};

#endif // BUFFERED_CHANGES_MAIN_H
