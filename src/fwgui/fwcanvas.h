#ifndef FIREWORKS_CANVAS_H
#define FIREWORKS_CANVAS_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qline.h>
#include <QtCore/qsharedpointer.h>

#include "fwgui/fwrender.h"

class FwPixmap;
class FwPixmapData;
class FwFont;

class FwCanvas
{
public:
    explicit FwCanvas();
    explicit FwCanvas(FwRender* render);
    explicit FwCanvas(const FwCanvas& rhs);
    virtual ~FwCanvas();

    inline bool isNull() const;

    QPoint startPos() const;

    QRect rect() const;
    void setRect(const QRect& rect);

    QPoint nullPos() const;
    void setNullPos(const QPoint& nullPos);

    void setColor(const FwColor& color);
    void setFont(const FwFont& font);

    void drawFillRect(const QRect& rect);
    void drawFillRect(const FwColor& color, const QRect& rect);

    void drawRect(const QRect& rect);
    void drawLine(const QLine& line);

    void drawPixmap(int x, int y, const FwPixmap& pixmap, const QRect& srcRect = QRect());
    void drawPixmap(const QRect& rect, const FwPixmap& pixmap, const QRect& srcRect = QRect());

    inline void drawSurface(const QPoint& pos, FwPixmapData* surface, const QRect* srcRect = 0);
    void drawSurface(int x, int y, FwPixmapData* surface, const QRect* scrRect = 0);
    void drawSurface(const QRect& rect, FwPixmapData* surface, const QRect* srcRect = 0);

    void drawString(int x, int y, const QString& string);
    void drawUtf8String(int x, int y, const QByteArray& utf8String);

    inline QRect saveClip() const;
    inline QRect setClip(const QRect& clipRect);
    inline QRect resetClip();
    inline void restoreClip(const QRect& clipRect);

    void flip(const QRect& rect);

    inline qreal opacity() const;
    void setOpacity(qreal opacity);

    void clear();

protected:
    void updateStartPos();

private:
    QSharedPointer<FwRender> render;
};

bool FwCanvas::isNull() const
{
    return render == 0;
}

qreal FwCanvas::opacity() const
{
    return render->opacity;
}

QRect FwCanvas::saveClip() const
{
    return render->saveClip();
}

QRect FwCanvas::resetClip()
{
    return setClip(QRect());
}

QRect FwCanvas::setClip(const QRect& clipRect)
{
    return render->setClip(clipRect);
}

void FwCanvas::restoreClip(const QRect& rect)
{
    render->restoreClip(rect);
}

void FwCanvas::drawSurface(const QPoint& pos, FwPixmapData* surface, const QRect* srcRect)
{
    render->drawSurface(pos.x(), pos.y(), surface, srcRect);
}

#endif // FWCANVAS_H
