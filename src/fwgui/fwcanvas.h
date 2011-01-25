#ifndef FIREWORKS_CANVAS_H
#define FIREWORKS_CANVAS_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qline.h>
#include <QtCore/qsharedpointer.h>

#include "fwgui/fwrender.h"
#include "fwgui/fwpixmap.h"

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

    inline void drawPixmap(int x, int y, const FwPixmap& pixmap);
    inline void drawPixmap(const QRect& rect, const FwPixmap& pixmap, const QRect* srcRect = 0);

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

    inline FwRender* render();

protected:
    void updateStartPos();

private:
    QSharedPointer<FwRender> m_render;
};

bool FwCanvas::isNull() const
{
    return m_render == 0;
}

qreal FwCanvas::opacity() const
{
    return m_render->opacity;
}

QRect FwCanvas::saveClip() const
{
    return m_render->saveClip();
}

QRect FwCanvas::resetClip()
{
    return setClip(QRect());
}

QRect FwCanvas::setClip(const QRect& clipRect)
{
    return m_render->setClip(clipRect);
}

void FwCanvas::restoreClip(const QRect& rect)
{
    m_render->restoreClip(rect);
}

FwRender* FwCanvas::render()
{
    return m_render.data();
}

void FwCanvas::drawPixmap(int x, int y, const FwPixmap& pixmap)
{
    if(!pixmap.isNull())
    {
        m_render->drawPixmap(m_render->startX() + x, m_render->startY() + y, pixmap.surface());
    }
}

void FwCanvas::drawPixmap(const QRect& rect, const FwPixmap& pixmap, const QRect* srcRect)
{
    if(srcRect && rect.size() == pixmap.size() && (*srcRect).size() == rect.size())
    {
        drawPixmap(rect.x(), rect.y(), pixmap);
        return;
    }
    if(!pixmap.isNull())
    {
        QRect r = rect.translated(m_render->startX(), m_render->startY());
        m_render->drawPixmap(r, pixmap.surface(), srcRect);
    }
}

#endif // FWCANVAS_H
