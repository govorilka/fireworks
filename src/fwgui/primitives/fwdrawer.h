#ifndef FIREWORKS_DRAWER_H
#define FIREWORKS_DRAWER_H

#include <QtCore/qrect.h>

#include "fwcore/fwcolor.h"

#include "fwgui/fwpixmap.h"

class FwPrimitive;
class FwPainter;
class FwVertexArray;
class FwMLObject;

class FwDrawer
{
public:
    FwDrawer(const FwPrimitive* primitive);

    inline const FwPrimitive* primitive() const;

    virtual void paint(FwPainter* painter, const QRect& clipRect) = 0;

    virtual void setPrimitiveRect(const QRect& rect) = 0;

    virtual void apply(FwMLObject* object) = 0;

private:
    const FwPrimitive* m_primitive;
};

const FwPrimitive* FwDrawer::primitive() const
{
    return m_primitive;
}

///////////////////////////////////////////////////////////////////

class FwRectDrawer : public FwDrawer
{
    typedef FwDrawer BaseClass;

public:
    FwRectDrawer(const FwPrimitive* primitive);

    void paint(FwPainter* painter, const QRect& clipRect);

    void setPrimitiveRect(const QRect& rect);

    inline FwColor color() const;
    inline void setColor(const FwColor& color);

    inline int penWidth() const;
    inline void setPenWidth(int width);

    void apply(FwMLObject* object);

protected:
    void createVertexArray();

    virtual void setupCoordinates(int &ax, int &ay, int &bx, int& by, const QRect& rect);
    virtual void updateVertexArray(const QRect& rect);

    int m_x;
    int m_y;
    int m_ax;
    int m_ay;
    int m_bx;
    int m_by;
    FwVertexArray* m_vertexes;

private:
    FwColor m_color;
    int m_penWidth;
};

FwColor FwRectDrawer::color() const
{
    return m_color;
}

void FwRectDrawer::setColor(const FwColor& color)
{
    m_color = color;
}

int FwRectDrawer::penWidth() const
{
    return m_penWidth;
}

void FwRectDrawer::setPenWidth(int width)
{
    m_penWidth = width;
}

///////////////////////////////////////////////////////////////////

class FwRoundedRectDrawer : public FwRectDrawer
{
    typedef FwRectDrawer BaseClass;
public:
    FwRoundedRectDrawer(int radius, const FwPrimitive* primitive);

    inline int radius() const;

protected:
    void setupCoordinates(int &ax, int &ay, int &bx, int& by, const QRect& rect);
    void updateVertexArray(const QRect& rect);

    void circlePlotPoints(const QPoint& circPt);

private:
    int m_radius;
};

int FwRoundedRectDrawer::radius() const
{
    return m_radius;
}

///////////////////////////////////////////////////////////////////

class FwBorderImageDrawer : public FwRectDrawer
{
    typedef FwRectDrawer BaseClass;

public:

    FwBorderImageDrawer(const FwPrimitive* primitive);

    void paint(FwPainter* painter, const QRect& clipRect);

    void setPrimitiveRect(const QRect& rect);

    void apply(FwMLObject* object);

    inline FwPixmap topLeft() const;
    inline void setTopLeft(const FwPixmap& pixmap);

    inline FwPixmap topRight() const;
    inline void setTopRight(const FwPixmap& pixmap);

    inline FwPixmap bottomLeft() const;
    inline void setBottomLeft(const FwPixmap& pixmap);

    inline FwPixmap bottomRight() const;
    inline void setBottomRight(const FwPixmap& pixmap);

protected:
    void setupCoordinates(int &ax, int &ay, int &bx, int& by, const QRect& rect);
    void updateVertexArray(const QRect& rect);

private:
    FwPixmap m_topLeft;
    FwPixmap m_topRight;
    FwPixmap m_bottomLeft;
    FwPixmap m_bottomRight;
    int m_lineLeftMargin;
    int m_lineRightMargin;
    int m_lineTopMargin;
    int m_lineBottomMargin;
};

FwPixmap FwBorderImageDrawer::topLeft() const
{
    return m_topLeft;
}

void FwBorderImageDrawer::setTopLeft(const FwPixmap& pixmap)
{
    m_topLeft = pixmap;
}

FwPixmap FwBorderImageDrawer::topRight() const
{
    return m_topRight;
}

void FwBorderImageDrawer::setTopRight(const FwPixmap& pixmap)
{
    m_topRight = pixmap;
}

FwPixmap FwBorderImageDrawer::bottomLeft() const
{
    return m_bottomLeft;
}

void FwBorderImageDrawer::setBottomLeft(const FwPixmap& pixmap)
{
     m_bottomLeft = pixmap;
}

FwPixmap FwBorderImageDrawer::bottomRight() const
{
    return m_bottomRight;
}

void FwBorderImageDrawer::setBottomRight(const FwPixmap& pixmap)
{
    m_bottomRight = pixmap;
}

#endif // FIREWORKS_DRAWER_H
