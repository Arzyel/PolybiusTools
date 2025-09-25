#pragma once
#include <QtWidgets\qapplication.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qboxlayout.h>
#include <QtGui\qpainter.h>
#include <QtGui\qevent.h>
#include <iostream>
#include <string>


class ClickableCircle : public QWidget
{

public:

    ClickableCircle(QWidget* parent = nullptr) : QWidget(parent)
    {
        setFixedSize(200, 200); // Size of the widget
        color = Qt::blue;
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.setBrush(color);
        painter.setPen(Qt::black);
        painter.drawEllipse(20, 20, 160, 160); // x, y, width, height
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        // Check if click is inside the circle
        QPointF center(width() / 2, height() / 2);
        double radius = 80;
        if (QLineF(center, event->pos()).length() <= radius)
        {
            color = Qt::red;
        }
        else {
            color = Qt::yellow;
        }
        update();
    }
private:
    QColor color;
};

int exercice3(int argc, char* argv[]) {
    QApplication app(argc, argv);

    ClickableCircle circle;
    circle.show();

    return app.exec();

}
