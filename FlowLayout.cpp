#include "FlowLayout.h"


FlowLayout::FlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    QLayoutItem* item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem* item)
{
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    return m_hSpace >= 0 ? m_hSpace : smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
    return m_vSpace >= 0 ? m_vSpace : smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem* FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem* FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return {};
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), true);
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    for (QLayoutItem* item : std::as_const(itemList))
        size = size.expandedTo(item->minimumSize());

    // replace margin() with actual contentsMargins
    QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());

    return size;
}


QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

void FlowLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

int FlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
    int x = rect.x() + contentsMargins().left();
    int y = rect.y();
    int lineHeight = 0;

    int spaceX = horizontalSpacing();
    int spaceY = verticalSpacing();

    if (spaceX == -1) spaceX = 5;
    if (spaceY == -1) spaceY = 5;

    for (QLayoutItem* item : std::as_const(itemList)) {
        QWidget* wid = item->widget();
        QSize itemSize = item->sizeHint();

        if (x + itemSize.width() > rect.right() && lineHeight > 0) {
            x = rect.x() + contentsMargins().left();
            y += lineHeight + spaceY;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), itemSize));

        x += itemSize.width() + spaceX;
        lineHeight = qMax(lineHeight, itemSize.height());
    }

    return y + lineHeight - rect.y();
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject* parentObj = parent();
    if (!parentObj)
        return -1;
    if (parentObj->isWidgetType())
        return static_cast<QWidget*>(parentObj)->style()->pixelMetric(pm, nullptr, static_cast<QWidget*>(parentObj));
    return static_cast<QLayout*>(parentObj)->spacing();
}
