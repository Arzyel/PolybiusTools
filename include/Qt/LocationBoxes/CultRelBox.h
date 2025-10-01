#pragma once
#ifndef CULT_REL_BOX_H
#define CULT_REL_BOX_H
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtWidgets\qcombobox.h>
#include <QtWidgets\qlabel.h>

class CultRelBox : public QGroupBox{
public:
    CultRelBox() = delete;
    ~CultRelBox() = default;
    CultRelBox(const QString& title, QWidget* parent = nullptr);
    void loadWidget();
protected:

private:
    QComboBox* cultCBox;
    QComboBox* relCBox;
};

#endif // CULT_REL_BOX_H
