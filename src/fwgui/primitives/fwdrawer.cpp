#include <QtCore/qdebug.h>

#include "fwdrawer.h"

#include "fwgui/fwpainter.h"

FwDrawer::FwDrawer(const FwPrimitive* primitive) :
    m_primitive(primitive)
{
}

///////////////////////////////////////////////////////////////////

FwRoundedRectDrawer::FwRoundedRectDrawer(int radius, const FwPrimitive* primitive) :
    BaseClass(primitive),
    m_radius(radius),
    m_ax(0),
    m_ay(0),
    m_bx(0),
    m_by(0)
{
}

void FwRoundedRectDrawer::paint(FwPainter* painter, const QRect& clipRect)
{
    roundedRectMidpoint(painter, clipRect.normalized(), m_radius);
}

void FwRoundedRectDrawer::roundedRectMidpoint(FwPainter* painter, const QRect& rect, int radius)
{
    m_ax = rect.left() + m_radius;
    m_ay = rect.top() + m_radius;

    m_bx = rect.right() - m_radius;
    m_by = rect.bottom() - m_radius;

    painter->setColor(255, 255, 255, 255);

    for(int r = radius; r > (radius - 5); --r)
    {
        QPoint circPt(0, r);
        int p = 1 - r;

        circlePlotPoints(painter, circPt);
        while(circPt.x() < circPt.y())
        {
            circPt.rx()++;
            if(p < 0)
            {
                p += 2 * circPt.x() + 1;
            }
            else
            {
                circPt.ry()--;
                p += 2 * (circPt.x() - circPt.y()) + 1;
            }
            qDebug() << circPt;
            circlePlotPoints(painter, circPt);
        }

        int delta = radius - r;
        painter->drawLine(QLine(m_ax, rect.top() + delta, m_bx, rect.top() + delta));
        painter->drawLine(QLine(m_ax, rect.bottom() - delta, m_bx, rect.bottom() - delta));
        painter->drawLine(QLine(rect.left() + delta, m_ay, rect.left() + delta, m_by));
        painter->drawLine(QLine(rect.right() - delta, m_ay, rect.right() - delta, m_by));
    }


    /*painter->drawLine(QLine(m_ax, rect.top(), m_bx, rect.top()));
    painter->drawLine(QLine(m_ax, rect.bottom(), m_bx, rect.bottom()));
    painter->drawLine(QLine(rect.left(), m_ay, rect.left(), m_by));
    painter->drawLine(QLine(rect.right(), m_ay, rect.right(), m_by));*/

    /*painter->drawLine(QLine(m_ax, rect.top() + 10, m_bx, rect.top() + 10));
    painter->drawLine(QLine(m_ax, rect.bottom() - 10, m_bx, rect.bottom() - 10));
    painter->drawLine(QLine(rect.left() + 10, m_ay, rect.left() + 10, m_by));
    painter->drawLine(QLine(rect.right() - 10, m_ay, rect.right() - 10, m_by));*/

}

void FwRoundedRectDrawer::circlePlotPoints(FwPainter* painter, const QPoint& circPt)
{
    painter->drawLine(QLine(m_ax - circPt.x(), m_ay - circPt.y(), m_ax - circPt.x(), m_ay - circPt.y()));
    painter->drawLine(QLine(m_ax - circPt.y(), m_ay - circPt.x(), m_ax - circPt.y(), m_ay - circPt.x()));

    painter->drawLine(QLine(m_ax - circPt.x(), m_by + circPt.y(), m_ax - circPt.x(), m_by + circPt.y()));
    painter->drawLine(QLine(m_ax - circPt.y(), m_by + circPt.x(), m_ax - circPt.y(), m_by + circPt.x()));

    painter->drawLine(QLine(m_bx + circPt.x(), m_by + circPt.y(), m_bx + circPt.x(), m_by + circPt.y()));
    painter->drawLine(QLine(m_bx + circPt.y(), m_by + circPt.x(), m_bx + circPt.y(), m_by + circPt.x()));

    painter->drawLine(QLine(m_bx + circPt.x(), m_ay - circPt.y(), m_bx + circPt.x(), m_ay - circPt.y()));
    painter->drawLine(QLine(m_bx + circPt.y(), m_ay - circPt.x(), m_bx + circPt.y(), m_ay - circPt.x()));
}
