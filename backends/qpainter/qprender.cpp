#include <QtGui/qbrush.h>

#include "qprender.h"
#include "qpgraphicsview.h"

QPRender::QPRender(QPWidget* w) :
    BaseClass(),
    m_painter(w),
    m_color(0x00, 0x00, 0x00, 0x00)
{
}

void QPRender::setColor(const FwColor& color)
{
    m_color = QColor(color.red(), color.green(), color.blue(), color.alpha());
}

void QPRender::setFont(const FwFont& font)
{
}

void QPRender::drawFillRect(int x, int y, int w, int h)
{
    m_painter.setPen(m_color);
    m_painter.setBrush(QBrush(m_color));
    m_painter.drawRect(x, y, w, h);
}

void QPRender::prepareDrawSurface(FwPixmapData* surface)
{
}

void QPRender::drawSurface(int x, int y, FwPixmapData* surface, const QRect* srcRect)
{
}

void QPRender::drawSurface(const QRect& rect, FwPixmapData* surface, const QRect* srcRect)
{
}

void QPRender::drawRect(int x, int y, int w, int h)
{
}

void QPRender::drawLine(int x1, int y1, int x2, int y2)
{
}

void QPRender::flip(int x, int y, int w, int h)
{
}

void QPRender::drawString(int x, int y, const QString& string)
{
}

void QPRender::drawString(int x, int y, const char* str, int strSize)
{
}

void QPRender::updateClipRect(const QRect& rect)
{
}

void QPRender::clear()
{
}
