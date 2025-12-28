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
#include <QtCore/qtimer.h>
#include <QEvent>
#include <unordered_map>
#include <unordered_set>
#include "FileManager.h"

struct CachedWidget {
    QListWidgetItem* item = nullptr;
    QGroupBox* group = nullptr;
    QTableWidget* table = nullptr;
    std::unordered_map<uint16_t, int> indexToRow;  // Maps token index to table row
    std::unordered_map<uint16_t, std::pair<std::string, std::string>> cachedValues;  // index -> (origin, current)
    bool isCreated = false;
    DM::iFileDataBase* fileData = nullptr;
};
class BufferTab : public QWidget {
    Q_OBJECT
public:
    BufferTab(std::unordered_set<DM::iFileDataBase*>& setAChanges, QWidget* parent = nullptr);
    ~BufferTab() = default;
    void loadWidgets();
    void updateBufferWidgets();
protected:

private slots:
    void onScroll();
private:
    std::unordered_set<DM::iFileDataBase*>& activeChanges;
    QListWidget* mList;
    std::unordered_map<DM::iFileDataBase*, CachedWidget> mWidgetCache;
    void createSingleBufferWidget(QGroupBox* group, QVBoxLayout* groupLayout);
    // Claude ai solution for performance
    CachedWidget createWidgetForFileData(DM::iFileDataBase* fileData);
    void populateTable(DM::iFileDataBase* fileData, CachedWidget& cached);
    void updateWidgetSmartly(DM::iFileDataBase* fileData, CachedWidget& cached);
    void updateChangedCells(DM::iFileDataBase* fileData, CachedWidget& cached,
        const std::unordered_set<uint16_t>& currentIndices);
    void createWidgetForItem(DM::iFileDataBase* fileData, CachedWidget& cached);
    void destroyWidget(CachedWidget& cached);
    void updateVisibleWidgets();
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // BUFFERED_CHANGES_MAIN_H
