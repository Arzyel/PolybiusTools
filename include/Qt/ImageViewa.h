#pragma once
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include <QtGui\qaction.h>
#include <QtWidgets\qscrollbar.h>
#include <QtCore\qdebug.h>
#include <QtWidgets\qgraphicsview.h>
#include <QtWidgets\qgraphicswidget.h>
#include <QtGui\qpainter.h>
#include <queue>
#include <stack>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>
#include "Eu4GeoPolData.h"
#include "InformationGUI.h"
#include "FileOpener.h"

struct PixelPos { uint16_t x, y; };




class ImageView : public QGraphicsView {
public:
    ImageView(const QString& imagePath, const Eu4::GeoPolData& geoPolContainers, const InformationGUI& informationGUI,QWidget* parent = nullptr);
    ~ImageView();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void precomputeOverlays(const QVector<QRgb>& ruleRGBs, const QColor& overlayColor);
    void setActiveOverlay(const int& index);
    void createSelectionOverlay(QRgb rgb);
    QVector<PixelPos> getBorderPixels(const QVector<PixelPos>& pixels, int width, int height);
private:
    void precomputeColorMap();
    void precomputeColorMapOMP();
    void createOverlayForColor(QRgb rgb);


    QImage overlayImage;
    QHash<QRgb, QVector<PixelPos>> colorMap;
    QVector<QPoint> borderPixels;
    QColor overlayColor = Qt::white;

    QGraphicsScene* scene = nullptr;
    QGraphicsPixmapItem* pixmapItem = nullptr;
    double scaleFactor;
    bool dragging = false;
    QPoint lastPos;

    QImage selectionOverlay;
    int activeMapModeOverlay = -1;
    QVector<QImage> mapModeOverlays;
    QVector<QColor> mapModeColors;
    const Eu4::GeoPolData& mRefGeoPolCont;
    const InformationGUI& mRefInfoGUI;
};

#endif