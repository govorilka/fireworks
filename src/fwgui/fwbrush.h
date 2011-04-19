#ifndef FIREWORKS_BRUSH_H
#define FIREWORKS_BRUSH_H

#include <QtCore/qrect.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qdebug.h>

#include "fwcore/fwcolor.h"

#include "fwgui/fwpixmap.h"
#include "fwgui/fwpen.h"

class FwPainter;
class FwPrimitive;

class FwBrush
{
public:
    FwBrush(FwPrimitive* primitive);
    virtual ~FwBrush();

    inline FwPrimitive* primitive() const;

    inline FwPen* border() const;
    void setBorder(FwPen* border);

    void drawRect(FwPainter* painter, const QRect& clipRect);

    inline QRect sourceRect() const;
    void setSourceRect(const QRect& rect);

protected:
    virtual void drawBackground(FwPainter* painter, const QRect& clipRect) = 0;

    virtual void updateSourceRect(const QRect& rect) = 0;

    inline void updateGeometry();

private:
    QRect m_sourceRect;
    QRect m_backgroundRect;
    FwPen* m_border;
    FwPrimitive* m_primitive;
};

FwPen* FwBrush::border() const
{
    return m_border;
}

QRect FwBrush::sourceRect() const
{
    return m_sourceRect;
}

void FwBrush::updateGeometry()
{
    m_backgroundRect = m_border ? m_border->clientArea(m_sourceRect) : m_sourceRect;
    updateSourceRect(m_backgroundRect);
}

FwPrimitive* FwBrush::primitive() const
{
    return m_primitive;
}

///////////////////////////////////////////////////////////////////////////////

class FwBrushSolid : public FwBrush
{
    typedef FwBrush BaseClass;
public:
    FwBrushSolid(FwPrimitive* primitive, const FwColor& color);

    inline FwColor color() const;

protected:
    void drawBackground(FwPainter* painter, const QRect& clipRect);

    void updateSourceRect(const QRect& rect);

private:
    FwColor m_color;
};

///////////////////////////////////////////////////////////////////////////////

class FwBrushTexture : public FwBrush
{
    typedef FwBrush BaseClass;

public:
    FwBrushTexture(FwPrimitive* primitive, const FwPixmap& pixmap);

    inline FwPixmap pixmap() const;

protected:
    void drawBackground(FwPainter* painter, const QRect& clipRect);

    void updateSourceRect(const QRect& rect);

private:
    FwPixmap m_pixmap;
    FwPixmap m_displayPixmap;
};

FwPixmap FwBrushTexture::pixmap() const
{
    return m_pixmap;
}

#endif // FIREWORKS_BRUSH_H
