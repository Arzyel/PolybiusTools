#include "ImageViewa.h"


// ----------------------- CONSTRUCTOR -----------------------
ImageView::ImageView(const QString& imagePath, const Eu4::GeoPolData& geoPolContainers, const InformationGUI& informationGUI, QWidget* parent)
    : QGraphicsView(parent), mRefGeoPolCont(geoPolContainers), mRefInfoGUI(informationGUI)
{
    QPixmap pix(imagePath);
    if (pix.isNull()) {
        qDebug() << "Failed to load image!";
        return;
    }

    scene = new QGraphicsScene(this);
    pixmapItem = scene->addPixmap(pix);
    setScene(scene);

    setDragMode(QGraphicsView::NoDrag);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setAlignment(Qt::AlignCenter);

    fitInView(pixmapItem->boundingRect(), Qt::KeepAspectRatio);
    scaleFactor = 1.0;

    //precomputeColorMapOMP();
    precomputeColorMap();
    precomputeOverlays({ qRgb(202,46,173), qRgb(74,48,32), qRgb(148,133,60)}, Qt::black);
    setActiveOverlay(0);
}

ImageView::~ImageView()
{

}

// ----------------------- PRECOMPUTE COLOR MAP -----------------------
void ImageView::precomputeColorMap() {
    QImage img = pixmapItem->pixmap().toImage().convertToFormat(QImage::Format_RGB888);
    const int width = img.width();
    const int height = img.height();
    const int stride = img.bytesPerLine();
    const uchar* bits = img.constBits();
    colorMap.clear(); // clear any previous data
    constexpr int reserveSize = 2000; // preallocate this many pixels per color

    qDebug() << "Precomputing color map...";
    auto start = std::chrono::high_resolution_clock::now();


    for (int y = 0; y < height; ++y) {
        const uchar* row = bits + y * stride;
        for (int x = 0; x < width; ++x) {
            QRgb rgb = qRgb(row[x * 3], row[x * 3 + 1], row[x * 3 + 2]);
            colorMap[rgb].append({ uint16_t(x), uint16_t(y) });
        }
    }


    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    qDebug() << "Color map ready. Unique colors:" << colorMap.size()
        << "Elapsed time:" << elapsed.count() << "ms";
}

void ImageView::precomputeColorMapOMP()
{

        QImage img = pixmapItem->pixmap().toImage().convertToFormat(QImage::Format_RGB888);
        const int width = img.width();
        const int height = img.height();
        const int stride = img.bytesPerLine();
        const uchar* bits = img.constBits();

        qDebug() << "Precomputing color map (OpenMP, optimized for large image)...";
        auto start = std::chrono::high_resolution_clock::now();

        colorMap.clear();

        constexpr int reserveSize = 200; // preallocate per color
        int numThreads = omp_get_max_threads();

        // Thread-local color maps
        std::vector<QHash<QRgb, QVector<PixelPos>>> threadMaps(numThreads);

        // Parallel loop with row blocks
#pragma omp parallel for schedule(static, 32)  // 32 rows per chunk
        for (int y = 0; y < height; ++y) {
            int tid = omp_get_thread_num();
            auto& localMap = threadMaps[tid];
            const uchar* row = bits + y * stride;

            for (int x = 0; x < width; ++x) {
                QRgb rgb = qRgb(row[x * 3], row[x * 3 + 1], row[x * 3 + 2]);
                auto& vec = localMap[rgb];
                if (vec.isEmpty()) vec.reserve(reserveSize);
                vec.append({ uint16_t(x), uint16_t(y) });
            }
        }

        // Merge thread-local maps into main colorMap
        for (auto& tmap : threadMaps) {
            for (auto it = tmap.begin(); it != tmap.end(); ++it) {
                colorMap[it.key()].append(it.value().constBegin(), it.value().constEnd());
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        qDebug() << "Color map ready. Unique colors:" << colorMap.size()
            << "Elapsed time:" << elapsed.count() << "ms";

}


// ----------------------- CREATE OVERLAY -----------------------
void ImageView::createOverlayForColor(QRgb rgb) {
    if (!pixmapItem) return;

    const QSize size = pixmapItem->pixmap().size();

    // 1. Create transparent overlay
    overlayImage = QImage(size, QImage::Format_ARGB32);
    overlayImage.fill(Qt::transparent);

    // 2. Prepare solid overlay color
    QRgb overlayRgb = qRgb(overlayColor.red(), overlayColor.green(), overlayColor.blue());

    // 3. Fill only pixels of selected color
    const QVector<PixelPos>& pixels = colorMap.value(rgb);
    for (const PixelPos& pos : pixels) {
        overlayImage.setPixel(pos.x, pos.y, overlayRgb | 0xFF000000); // alpha 255
    }
}
void ImageView::createSelectionOverlay(QRgb rgb) {
    if (!pixmapItem) return;

    const QSize size = pixmapItem->pixmap().size();
    const int width = size.width();
    const int height = size.height();

    // 1. Create transparent overlay
    overlayImage = QImage(size, QImage::Format_ARGB32);
    overlayImage.fill(Qt::transparent);

    // 2. Prepare solid overlay color
    QRgb overlayRgb = qRgb(overlayColor.red(), overlayColor.green(), overlayColor.blue()) | 0xFF000000; // alpha 255

    // 3. Get pixels for the selected color
    const QVector<PixelPos>& pixels = colorMap.value(rgb);

    // 4. Compute border pixels only
    QVector<PixelPos> borderPixels = getBorderPixels(pixels, width, height);

    // 5. Set only border pixels in the overlay
    for (const PixelPos& pos : borderPixels) {
        overlayImage.setPixel(pos.x, pos.y, overlayRgb);
    }
}

// ----------------------- WHEEL & MOUSE -----------------------
void ImageView::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        double factor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
        scale(factor, factor);
        scaleFactor *= factor;
        event->accept();
    }
    else {
        QGraphicsView::wheelEvent(event);
    }
}

void ImageView::mouseMoveEvent(QMouseEvent* event) {
    if (dragging) {
        QPoint delta = lastPos - event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() + delta.y());
        lastPos = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ImageView::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QPointF pixmapPos = pixmapItem->mapFromScene(scenePos);

        int x = qBound(0, int(std::floor(pixmapPos.x())), pixmapItem->pixmap().width() - 1);
        int y = qBound(0, int(std::floor(pixmapPos.y())), pixmapItem->pixmap().height() - 1);

        QImage img = pixmapItem->pixmap().toImage().convertToFormat(QImage::Format_RGB888);
        const uchar* p = img.constBits() + y * img.bytesPerLine() + x * 3;
        QRgb clickedRgb = qRgb(p[0], p[1], p[2]);

        auto UID = mRefGeoPolCont.getIDFromColor((p[0] << 16) | (p[1] << 8) | p[2]);
        auto& prov = mRefGeoPolCont.getProvinceData(UID);
        qDebug() << "Clicked at pixel:" << x << y << "RGB:" << p[0] << p[1] << p[2] 
            << "\nProvince ID : " << UID
            <<"Province Name : " << prov.mName;

        mRefInfoGUI.loadProvInfo(prov);

        overlayColor = Qt::white;  // change if you want a different overlay
        createSelectionOverlay(clickedRgb);

        viewport()->update(); // redraw overlay
    }
    else if (event->button() == Qt::MiddleButton) {
        dragging = true;
        lastPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void ImageView::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF scenePos = mapToScene(event->pos());
        QPointF pixmapPos = pixmapItem->mapFromScene(scenePos);

        int x = qBound(0, int(std::floor(pixmapPos.x())), pixmapItem->pixmap().width() - 1);
        int y = qBound(0, int(std::floor(pixmapPos.y())), pixmapItem->pixmap().height() - 1);

        QImage img = pixmapItem->pixmap().toImage().convertToFormat(QImage::Format_RGB888);
        const uchar* p = img.constBits() + y * img.bytesPerLine() + x * 3;
        QRgb clickedRgb = qRgb(p[0], p[1], p[2]);

        auto UID = mRefGeoPolCont.getIDFromColor((p[0] << 16) | (p[1] << 8) | p[2]);
        auto& prov = mRefGeoPolCont.getProvinceData(UID);
        FileOpener::openTextFile(prov.mFilePath);
    }

    QGraphicsView::mouseDoubleClickEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MiddleButton) {
        dragging = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

// ----------------------- DRAW OVERLAY -----------------------
void ImageView::drawForeground(QPainter* painter, const QRectF&) {


    // Save previous render hint
    bool oldHint = painter->testRenderHint(QPainter::SmoothPixmapTransform);

    // Disable smooth scaling for pixel-perfect overlay
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    // Draw the overlay

    if (activeMapModeOverlay >= 0) {
        painter->drawImage(0, 0, mapModeOverlays[activeMapModeOverlay]);
    }

    if (!overlayImage.isNull()) {
        painter->drawImage(0, 0, overlayImage);
    }
    // Restore previous hint
    painter->setRenderHint(QPainter::SmoothPixmapTransform, oldHint);
}



void ImageView::precomputeOverlays(const QVector<QRgb>& ruleRGBs, const QColor& overlayColor)
{
    const QSize size = pixmapItem->pixmap().size();
    QImage overlay(size, QImage::Format_ARGB32);
    overlay.fill(Qt::transparent);

    QRgb color = qRgb(overlayColor.red(), overlayColor.green(), overlayColor.blue());

    for (QRgb rgb : ruleRGBs) {
        const QVector<PixelPos>& pixels = colorMap.value(rgb);
        for (const PixelPos& pos : pixels) {
            overlay.setPixel(pos.x, pos.y, color | 0xFF000000);
        }
    }

    mapModeOverlays.append(std::move(overlay));
}

void ImageView::setActiveOverlay(const int& index)
{
    if (index < 0 || index >= mapModeOverlays.size()) {
        activeMapModeOverlay = -1;
    }
    else {
        activeMapModeOverlay = index;
    }
    viewport()->update();
}

QVector<PixelPos> ImageView::getBorderPixels(const QVector<PixelPos>& pixels, int width, int height)
{
    // 1. Flat uint8_t mask (1 byte per pixel)
    std::vector<uint8_t> mask(width * height, 0);
    for (const auto& p : pixels)
        mask[p.y * width + p.x] = 1;

    QVector<PixelPos> borders;

    // 2. Neighbor offsets
    const int dx[4] = { 0, 0, -1, 1 };
    const int dy[4] = { -1, 1, 0, 0 };

    // 3. Check each pixel
    for (const auto& p : pixels) {
        bool isBorder = false;
        int idx = p.y * width + p.x;

        for (int k = 0; k < 4; ++k) {
            int nx = p.x + dx[k];
            int ny = p.y + dy[k];

            if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
                isBorder = true;
                break;
            }

            if (mask[ny * width + nx] == 0) {
                isBorder = true;
                break;
            }
        }

        if (isBorder)
            borders.append(p);
    }

    return borders;
}

