#include <QtGui/qpainter.h>
#include <QtGui/qbrush.h>

#include "qppixmapdata.h"
#include "qprender.h"

QPPixmapData::QPPixmapData(const QPixmap& pixmap, const QString& fileName) :
   BaseClass(fileName),
   m_pixmap(pixmap)
{
}

void QPPixmapData::release()
{
}

QSize QPPixmapData::size() const
{
    return m_pixmap.size();
}

FwPixmapData* QPPixmapData::tiled(const QSize& size) const
{
    QPixmap newPixmap(size);
    newPixmap.fill(Qt::transparent);

    QPainter painter(&newPixmap);
    painter.setBrush(QBrush(m_pixmap));
    painter.drawRect(QRect(0, 0, size.width(), size.height()));

    return new QPPixmapData(newPixmap, "");
}

FwPixmapData* QPPixmapData::resized(const QSize& size) const
{
    QPixmap newPixmap(size);
    newPixmap.fill(Qt::transparent);

    QPainter painter(&newPixmap);
    painter.drawPixmap(0, 0, m_pixmap);

    return new QPPixmapData(newPixmap, "");
}

FwPixmapData* QPPixmapData::scaled(const QSize& size) const
{
    QPixmap newPixmap(size);
    newPixmap.fill(Qt::transparent);

    QPainter painter(&newPixmap);
    painter.drawPixmap(0, 0, size.width(), size.height(), m_pixmap);

    return new QPPixmapData(newPixmap, "");
}

FwRender* QPPixmapData::createRender()
{
    return new QPRender(&m_pixmap);
}
