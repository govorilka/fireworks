#include "fwpen.h"
#include "fwpainter.h"

#include "fwcore/fwml.h"

FwShadow::FwShadow(const QPoint& pos, const FwColor& color) :
    m_pos(pos),
    m_color(color)
{
}

////////////////////////////////////////////////////////////////////////////////

FwPen::FwPen(int width, const FwColor& color) :
    m_width(width),
    m_color(color),
    m_shadow(0)
{
}

FwPen::~FwPen()
{
    delete m_shadow;
}

void FwPen::drawLine(FwPainter* painter, const QLine& line)
{
    painter->setColor(m_color);

    if(!line.dx())
    {
        painter->drawFillRect(QRect(line.p1(), QSize(m_width, line.dy())));
    }
    else if(!line.dy())
    {
        painter->drawFillRect(QRect(line.p1(), QSize(line.dx(), m_width)));
    }
    else
    {
        painter->drawLine(line); //TODO!!!
    }
}

void FwPen::drawStroke(FwPainter* painter, const QRect& rect)
{
    painter->setColor(m_color);
    QRect r = rect;
    for(int i = 0; i < m_width; i++)
    {
        painter->drawRect(r);
        r.adjust(1, 1, -1, -1);
    }
}

void FwPen::drawString(FwPainter* painter, const QPoint& pos, const QByteArray& utf8String)
{
    if(m_shadow)
    {
        painter->setColor(m_shadow->color());
        painter->drawString(pos.x() + m_shadow->pos().x(),
                            pos.y() + m_shadow->pos().y(),
                            utf8String);
    }
    painter->setColor(m_color);
    painter->drawString(pos.x(), pos.y(), utf8String);
}

void FwPen::setShadow(FwShadow* shadow)
{
    if(m_shadow != shadow)
    {
        delete m_shadow;
        m_shadow = shadow;
    }
}

QRect FwPen::stringRect(const FwFont& font, const QString& string)
{
    if(!font.isNull() && !string.isEmpty())
    {
        QSize size = font.stringSize(string);
        size.setHeight(font.height());

        int x = 0;
        int y = 0;

        if(m_shadow)
        {
            if(m_shadow->pos().x() > 0)
            {
                size.setWidth(size.width() + m_shadow->pos().x());
            }
            else
            {
                x = m_shadow->pos().x();
            }
            if(m_shadow->pos().y() > 0)
            {
                size.setHeight(size.height() + m_shadow->pos().y());
            }
            else
            {
                y = m_shadow->pos().y();
            }
        }

        return QRect(QPoint(x, y), size);
    }
    return QRect(0, 0, 0, 0);
}
