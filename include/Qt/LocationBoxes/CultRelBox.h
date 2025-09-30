#pragma once
#ifndef CULT_REL_BOX_H
#define CULT_REL_BOX_H
#include <QtWidgets\qgroupbox.h>

class CultRelBox : public QGroupBox{
public:
    CultRelBox() = delete;
    ~CultRelBox() = default;
    CultRelBox(const QString& title, QWidget* parent = nullptr);
protected:

private:
};

#endif // CULT_REL_BOX_H
