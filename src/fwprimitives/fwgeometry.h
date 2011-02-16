#ifndef FIREWORKS_GEOMETRY_H
#define FIREWORKS_GEOMETRY_H

#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qvarlengtharray.h>

#include "fireworks.h"

class FwPrimitive;

class FwGeometry
{
public:
    friend class FwPrimitive;

    FwGeometry();
    ~FwGeometry();

    inline QRect rect() const;
    inline void setRect(int x, int y, int w, int h);
    void setRect(const QRect& rect);

    inline bool contains(FwPrimitive* anchor) const;

    inline QSize size() const;
    inline bool sizeIsNull() const;

    inline QPoint pos() const;

    inline bool isDirty() const;
    inline bool posChanged() const;
    inline bool sizeChanged() const;
    void apply();

private:
    QVarLengthArray<FwPrimitive*> anchors;
    QRect m_rect;
    bool m_posChanged;
    bool m_sizeChanged;
};

void FwGeometry::setRect(int x, int y, int w, int h)
{
    setRect(QRect(x, y, w, h));
}

QRect FwGeometry::rect() const
{
    return m_rect;
}

bool FwGeometry::contains(FwPrimitive* anchor) const
{
    const FwPrimitive* const* iterAnchor = anchors.data();
    for(int i = 0; i < anchors.size(); i++, iterAnchor++)
    {
        if((*iterAnchor) == anchor)
        {
            return true;
        }
    }
    return false;
}

bool FwGeometry::sizeIsNull() const
{
    return m_rect.size().width() == 0 || m_rect.size().height() == 0;
}

QSize FwGeometry::size() const
{
    return m_rect.size();
}

QPoint FwGeometry::pos() const
{
    return m_rect.topLeft();
}

bool FwGeometry::isDirty() const
{
    return m_posChanged || m_sizeChanged;
}

bool FwGeometry::posChanged() const
{
    return m_posChanged;
}

bool FwGeometry::sizeChanged() const
{
    return m_sizeChanged;
}

#endif // FIREWORKS_GEOMETRY_H
