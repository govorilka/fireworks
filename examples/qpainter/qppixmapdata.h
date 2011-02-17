#ifndef FIREWORKS_QPPIXMAPDATA_H
#define FIREWORKS_QPPIXMAPDATA_H

#include <QtGui/qpixmap.h>

#include "fwgui/fwpixmap.h"

class QPPixmapData : public FwPixmapData
{
    typedef FwPixmapData BaseClass;

public:
    QPPixmapData(const QPixmap& pixmap, const QString& fileName);

    inline QPixmap qpixmap() const;

    void release();

    QSize size() const;

    FwPixmapData* tiled(const QSize& size) const;
    FwPixmapData* resized(const QSize& size) const;
    FwPixmapData* scaled(const QSize& size) const;

    FwRender* createRender();

private:
    QPixmap m_pixmap;
};

QPixmap QPPixmapData::qpixmap() const
{
    return m_pixmap;
}

#endif // FIREWORKS_QPPIXMAPDATA_H
