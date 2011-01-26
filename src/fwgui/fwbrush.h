#ifndef FIREWORKS_BRUSH_H
#define FIREWORKS_BRUSH_H

#include <QtCore/qrect.h>
#include <QtCore/qsharedpointer.h>

#include "fwcore/fwcolor.h"

#include "fwgui/fwpen.h"
#include "fwgui/fwpixmap.h"

class FwPainter;

class FwBrush
{
public:
    FwBrush();
    virtual ~FwBrush();

    inline FwPenPtr border() const;
    inline void setBorder(FwPenPtr border);

    void drawRect(FwPainter* painter, const QRect& clipRect);

    virtual void setSourceRect(const QRect& rect);
    virtual QRect sourceRect() const;

protected:
    virtual void drawBackground(FwPainter* painter, const QRect& clipRect) = 0;

private:
    FwPenPtr m_border;
};

FwPenPtr FwBrush::border() const
{
    return m_border;
}

void FwBrush::setBorder(FwPenPtr border)
{
    m_border = border;
}

///////////////////////////////////////////////////////////////////////////////

class FwBrushSolid : public FwBrush
{
    typedef FwBrush BaseClass;
public:
    FwBrushSolid(const FwColor& color);

    inline FwColor color() const;

protected:
    void drawBackground(FwPainter* painter, const QRect& clipRect);

private:
    FwColor m_color;
};

///////////////////////////////////////////////////////////////////////////////

class FwBrushTexture : public FwBrush
{
    typedef FwBrush BaseClass;

public:
    FwBrushTexture(const FwPixmap& pixmap);

    inline FwPixmap pixmap() const;

    void setSourceRect(const QRect& rect);
    QRect sourceRect() const;

protected:
    void drawBackground(FwPainter* painter, const QRect& clipRect);

private:
    FwPixmap m_pixmap;
    FwPixmap m_displayPixmap;
    QRect m_sourceRect;
};

FwPixmap FwBrushTexture::pixmap() const
{
    return m_pixmap;
}

///////////////////////////////////////////////////////////////////////////////

typedef QSharedPointer<FwBrush> FwBrushPtr;

#endif // FIREWORKS_BRUSH_H
