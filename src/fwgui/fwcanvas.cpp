#include <QtCore/qdebug.h>

#include "fwcanvas.h"
#include "fwpixmap.h"

FwCanvas::FwCanvas() :
    render(0)
{
}

FwCanvas::FwCanvas(FwRender* r) :
    render(r)
{
    render->updateColor();
}

FwCanvas::FwCanvas(const FwCanvas& rhs) :
    render(rhs.render)
{
}

FwCanvas::~FwCanvas()
{
}

QPoint FwCanvas::startPos() const
{
    return QPoint(render->startX, render->startY);
}

QRect FwCanvas::rect() const
{
    return render->rect;
}

void FwCanvas::setRect(const QRect& rect)
{
    if(render->rect != rect)
    {
        render->rect = rect;
        updateStartPos();
    }
}

QPoint FwCanvas::nullPos() const
{
    return render->nullPos;
}

void FwCanvas::setNullPos(const QPoint& nullPos)
{
    if(render->nullPos != nullPos)
    {
        render->nullPos = nullPos;
        updateStartPos();
    }
}

void FwCanvas::setColor(const FwColor& color)
{
    if(render->colorBuffer != color)
    {
        render->colorBuffer = color;
        render->updateColor();
    }
}

void FwCanvas::setFont(const FwFont& font)
{
    render->setFont(font);
}

void FwCanvas::drawFillRect(const QRect& rect)
{
    render->drawFillRect(render->startX + rect.x(),
                         render->startY + rect.y(),
                         rect.width(),
                         rect.height());
}

void FwCanvas::drawFillRect(const FwColor& color, const QRect& rect)
{
    FwColor colorBuffer = render->colorBuffer;
    if(render->colorBuffer != color)
    {
        setColor(color);
        render->drawFillRect(render->startX + rect.x(),
                             render->startY + rect.y(),
                             rect.width(),
                             rect.height());
        setColor(colorBuffer);
    }
    else
    {
        render->drawFillRect(render->startX + rect.x(),
                             render->startY + rect.y(),
                             rect.width(),
                             rect.height());
    }
}

void FwCanvas::drawRect(const QRect& rect)
{
    render->drawRect(render->startX + rect.x(),
                     render->startY + rect.y(),
                     rect.width(),
                     rect.height());
}

void FwCanvas::drawString(int x, int y, const QString& string)
{
    render->drawString(render->startX + x,
                       render->startY + y,
                       string);
}

void FwCanvas::drawUtf8String(int x, int y, const QByteArray& utf8String)
{
    render->drawString(render->startX + x,
                       render->startY + y,
                       utf8String.data(),
                       utf8String.size());
}

void FwCanvas::drawPixmap(int x, int y, const FwPixmap &pixmap, const QRect& srcRect)
{
    if(!pixmap.isNull())
    {
        drawSurface(x,
                    y,
                    pixmap.surface(),
                    srcRect.isNull() ? 0 : &srcRect);
    }
}

void FwCanvas::drawPixmap(const QRect& rect, const FwPixmap& pixmap, const QRect& srcRect)
{
    if(rect.size() == pixmap.size())
    {
        drawPixmap(rect.x(), rect.y(), pixmap, srcRect);
        return;
    }
    if(!pixmap.isNull())
    {
        drawSurface(rect, pixmap.surface(), srcRect.isNull() ? 0 : &srcRect);
    }
}

void FwCanvas::drawSurface(int x, int y, FwPixmapData* surface, const QRect* srcRect)
{
    if(surface)
    {
        render->drawSurface(render->startX + x,
                            render->startY + y,
                            surface,
                            srcRect);
    }
}

void FwCanvas::drawSurface(const QRect& rect, FwPixmapData* surface, const QRect* srcRect)
{
    if(surface)
    {
        render->drawSurface(rect.translated(render->startX,
                                            render->startY),
                                            surface,
                                            srcRect);
    }
}

void FwCanvas::drawLine(const QLine& line)
{
    render->drawLine(line.x1(), line.y1(), line.x2(), line.y2());
}

void FwCanvas::flip(const QRect& rect)
{
    render->flip(render->startX + rect.x(),
                 render->startY + rect.y(),
                 rect.width(),
                 rect.height());
}

void FwCanvas::updateStartPos()
{
    render->startX = render->nullPos.x() - render->rect.x();
    render->startY = render->nullPos.y() - render->rect.y();
}

void FwCanvas::setOpacity(qreal opacity)
{
    if(render->opacity != opacity)
    {
        render->opacity = opacity;
        render->updateColor();
    }
}

void FwCanvas::clear()
{
    render->clear();
}
