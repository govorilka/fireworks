#include <QtCore/qdebug.h>

#include <QtGui/qbrush.h>
#include <QtGui/qpaintdevice.h>

#include "qprender.h"
#include "qpfontdata.h"
#include "qppixmapdata.h"

QPVertexArray::QPVertexArray() :
    BaseClass()
{
}

QPVertexArray::~QPVertexArray()
{
}

void QPVertexArray::begin()
{
    m_lines.clear();
}

void QPVertexArray::end()
{
}

void QPVertexArray::addLine(int x1, int y1, int x2, int y2)
{
    m_lines.append(QLine(x1, y1, x2, y2));
}

void QPVertexArray::addRect(int x, int y, int w, int h)
{
    m_rectangles.append(QRect(x, y, w, h));
}

/////////////////////////////////////////////////////////////////////////////////////

QPRender::QPRender(QPaintDevice* device) :
    BaseClass(),
    m_painter(device),
    m_color(0x00, 0x00, 0x00, 0x00)
{
    m_painter.setClipping(true);
}

void QPRender::setClipRect(const QRect& rect)
{
    m_painter.setClipRect(rect);
}

void QPRender::setColor(const FwColor& color)
{
    m_color = QColor(color.red(), color.green(), color.blue(), color.alpha());
    m_painter.setPen(m_color);
}

void QPRender::setOpacity(float opacity)
{
    m_painter.setOpacity(opacity);
}

void QPRender::setFont(FwFontData* font)
{
    m_painter.setFont(static_cast<QPFontData*>(font)->qfont());
}

void QPRender::resetFont()
{
    m_painter.setFont(QFont());
}

void QPRender::drawRect(const QRect& rect)
{
    m_painter.drawRect(rect);
}

void QPRender::drawFillRect(const QRect& rect)
{
    m_painter.fillRect(rect, m_color);
}

void QPRender::drawLine(const QLine& line)
{
    m_painter.drawLine(line);
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

void QPRender::drawString(int x, int y, const QByteArray& utf8String)
{
    m_painter.drawText(x, y, QString::fromUtf8(utf8String));
}

void QPRender::drawVertexArray(FwVertexArray* array)
{
    QPVertexArray* vertexArray = static_cast<QPVertexArray*>(array);
    m_painter.drawLines(vertexArray->m_lines);
    foreach(const QRect& rect, vertexArray->m_rectangles)
    {
        m_painter.fillRect(rect, m_color);
    }
}
