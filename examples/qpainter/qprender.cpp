#include <QtGui/qbrush.h>
#include <QtGui/qpaintdevice.h>

#include "qprender.h"
#include "qpfontdata.h"
#include "qppixmapdata.h"

QPRender::QPRender(QPaintDevice* device) :
    BaseClass(),
    m_painter(device),
    m_color(0x00, 0x00, 0x00, 0x00)
{
}

void QPRender::setColor(const FwColor& color)
{
    m_color = QColor(color.red(), color.green(), color.blue(), color.alpha());
}

void QPRender::setFont(const FwFont& font)
{
    m_painter.setFont(static_cast<QPFontData*>(font.data())->qfont());
}

void QPRender::drawFillRect(int x, int y, int w, int h)
{
    m_painter.setPen(m_color);
    m_painter.setBrush(QBrush(m_color));
    m_painter.drawRect(x, y, w, h);
}

void QPRender::prepareDrawSurface(FwPixmapData* surface)
{
    Q_UNUSED(surface);
}

void QPRender::drawPixmap(int x, int y, FwPixmapData* pixmap)
{
    m_painter.drawPixmap(x, y, static_cast<QPPixmapData*>(pixmap)->qpixmap());
}

void QPRender::drawPixmap(const QRect& rect, FwPixmapData* pixmap, const QRect* srcRect)
{
    if(srcRect)
    {
        m_painter.drawPixmap(rect,
                             static_cast<QPPixmapData*>(pixmap)->qpixmap(),
                             (*srcRect));
    }
    else
    {
        m_painter.drawPixmap(rect, static_cast<QPPixmapData*>(pixmap)->qpixmap());
    }
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
    m_painter.drawText(x, y, string);
}

void QPRender::drawString(int x, int y, const char* str, int strSize)
{
    m_painter.setPen(m_color);
    m_painter.drawText(x, y, QString::fromUtf8(str, strSize));
}

void QPRender::updateClipRect(const QRect& rect)
{
}

void QPRender::clear()
{
}
