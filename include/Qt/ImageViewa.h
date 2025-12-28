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
    Q_OBJECT
public:
    ImageView(const QString& imagePath, Eu4::GeoPolData& geoPolContainers, const InformationGUI& informationGUI,QWidget* parent = nullptr);
    ~ImageView();
    

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void precomputeOverlays();
    void setActiveOverlay(const int& index);
    void createSelectionOverlay(QRgb rgb, bool add = false);
    QVector<PixelPos> getBorderPixels(const QVector<PixelPos>& pixels, int width, int height);
private:
    void precomputeColorMap();
    void precomputeColorMapOMP();
    void createOverlayForColor(QRgb rgb);
    QVector<QRgb> generateSparseColorsLand(int numColors);
    QVector<QRgb> generateSparseColorsSea(int numColors);
    void createAllOverlays();


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
    Eu4::GeoPolData& mRefGeoPolCont;
    const InformationGUI& mRefInfoGUI;

public slots:
    void changeView(uint8_t type);
};

#endif