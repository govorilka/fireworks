#include <QtCore/qdebug.h>

#include "fwdrawer.h"
#include "fwprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"

FwDrawer::FwDrawer(const FwPrimitive* primitive) :
    m_primitive(primitive)
{
}

///////////////////////////////////////////////////////////////////

FwRectDrawer::FwRectDrawer(const FwPrimitive* primitive) :
    BaseClass(primitive),
    m_x(0),
    m_y(0),
    m_ax(0),
    m_ay(0),
    m_bx(0),
    m_by(0),
    m_vertexes(0),
    m_color(255, 255, 255, 255),
    m_penWidth(3)
{
}

void FwRectDrawer::paint(FwPainter* painter, const QRect& clipRect)
{
    if(m_vertexes)
    {
        painter->setColor(m_color);
        painter->drawVertexArray(m_vertexes);
    }
}

void FwRectDrawer::setPrimitiveRect(const QRect& rect)
{
    m_x = rect.x();
    m_y = rect.y();
    delete m_vertexes;
    m_vertexes = primitive()->scene()->view()->createVertexArray();
    if(m_vertexes)
    {
        setupCoordinates(m_ax, m_ay, m_bx, m_by, rect);
        m_vertexes->begin();
        updateVertexArray(rect);
        m_vertexes->end();
    }
}

void FwRectDrawer::setupCoordinates(int &ax, int &ay, int &bx, int& by, const QRect& rect)
{
    ax = rect.left();
    ay = rect.top();
    bx = rect.right();
    by = rect.bottom();
}

void FwRectDrawer::updateVertexArray(const QRect& rect)
{
    Q_UNUSED(rect);
}

void FwRectDrawer::apply(FwMLObject* object)
{
    FwMLNode* colorNode = object->attribute("bgcolor");
    if(colorNode)
    {
        bool bOk = false;
        FwColor color = colorNode->toColor(&bOk);
        if(bOk)
        {
            setColor(color);
        }
    }
}

///////////////////////////////////////////////////////////////////

FwRoundedRectDrawer::FwRoundedRectDrawer(int radius, const FwPrimitive* primitive) :
    BaseClass(primitive),
    m_radius(radius)
{
}

void FwRoundedRectDrawer::circlePlotPoints(const QPoint& circPt)
{
    m_vertexes->addLine(m_ax - circPt.x(),  m_ay - circPt.y(), m_ax - circPt.x(), m_by + circPt.y());
    m_vertexes->addLine(m_ax - circPt.y(),  m_ay - circPt.x(), m_ax - circPt.y(), m_by + circPt.x());
    m_vertexes->addLine(m_bx + circPt.x(),  m_by + circPt.y(), m_bx + circPt.x(), m_ay - circPt.y());
    m_vertexes->addLine(m_bx + circPt.y(),  m_by + circPt.x(), m_bx + circPt.y(), m_ay - circPt.x());
}

void FwRoundedRectDrawer::setupCoordinates(int &ax, int &ay, int &bx, int& by, const QRect& rect)
{
    ax = rect.left() + m_radius;
    ay = rect.top() + m_radius;
    bx = rect.right() - m_radius;
    by = rect.bottom() - m_radius;
}

void FwRoundedRectDrawer::updateVertexArray(const QRect& rect)
{
    QPoint circPt(0, m_radius);
    int p = 1 - m_radius;

    circlePlotPoints(circPt);
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
        circlePlotPoints(circPt);
    }

    m_vertexes->addRect(m_ax, rect.top(), m_bx - m_ax, rect.height());
}

///////////////////////////////////////////////////////////////////

FwBorderImageDrawer::FwBorderImageDrawer(const FwPrimitive* primitive) :
    BaseClass(primitive),
    m_lineLeftMargin(1),
    m_lineRightMargin(1),
    m_lineTopMargin(1),
    m_lineBottomMargin(0)
{
}

void FwBorderImageDrawer::paint(FwPainter* painter, const QRect& clipRect)
{
    BaseClass::paint(painter, clipRect);
    painter->drawPixmap(m_x, m_y, m_topLeft);
    painter->drawPixmap(m_bx, m_y, m_topRight);
    painter->drawPixmap(m_x, m_by, m_bottomLeft);
    painter->drawPixmap(m_bx, m_by, m_bottomRight);
}

void FwBorderImageDrawer::setPrimitiveRect(const QRect& rect)
{
    BaseClass::setPrimitiveRect(rect);
}

void FwBorderImageDrawer::apply(FwMLObject* object)
{
    FwMLNode* topLeftNode = object->attribute("topLeft");
    if(topLeftNode)
    {
        setTopLeft(primitive()->createPixmap(topLeftNode));
    }

    FwMLNode* topRightNode = object->attribute("topRight");
    if(topRightNode)
    {
        setTopRight(primitive()->createPixmap(topRightNode));
    }

    FwMLNode* bottomLeftNode = object->attribute("bottomLeft");
    if(bottomLeftNode)
    {
        setBottomLeft(primitive()->createPixmap(bottomLeftNode));
    }

    FwMLNode* bottomRightNode = object->attribute("bottomRight");
    if(bottomRightNode)
    {
        setBottomRight(primitive()->createPixmap(bottomRightNode));
    }
}

void FwBorderImageDrawer::setupCoordinates(int &ax, int &ay, int &bx, int& by, const QRect& rect)
{
    ax = rect.left() + m_topLeft.width();
    ay = rect.top() + m_topLeft.height();
    bx = rect.right() - m_bottomRight.width();
    by = rect.bottom() - m_bottomRight.height();
}

void FwBorderImageDrawer::updateVertexArray(const QRect& rect)
{
    QRect r = rect.adjusted(m_lineLeftMargin, m_lineTopMargin, -m_lineRightMargin, -m_lineBottomMargin);
    if(penWidth() > 1)
    {
        int pWidth = penWidth();
        m_vertexes->addRect(m_ax, r.top(), m_bx - m_ax, pWidth);
        m_vertexes->addRect(m_ax, r.bottom() - pWidth, m_bx - m_ax, pWidth);
        m_vertexes->addRect(r.left(), m_ay, pWidth, m_by - m_ay);
        m_vertexes->addRect(r.right() - pWidth, m_ay, pWidth, m_by - m_ay);
    }
    else
    {
        m_vertexes->addLine(m_ax, r.top(), m_bx, r.top());
        m_vertexes->addLine(m_ax, r.bottom(), m_bx, r.bottom());
        m_vertexes->addLine(r.left(), m_ay, r.left(), m_by);
        m_vertexes->addLine(r.right(), m_ay, r.right(), m_by);
    }
}
