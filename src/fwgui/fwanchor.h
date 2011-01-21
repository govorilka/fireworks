#ifndef FIREWORKS_ANCHOR_H
#define FIREWORKS_ANCHOR_H

#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qvarlengtharray.h>

#include "fireworks.h"

class FwAnchor;
class FwGeometry;
class FwPrimitive;

class FwAnchor
{
public:

    enum HorizontalPolicy
    {
        HP_Unchanged,
        HP_BeforeLeft,
        HP_Left,
        HP_Center,
        HP_CenterDock,
        HP_Right,
        HP_AfterRight
    };

    enum VerticalPolicy
    {
        VP_Unchanged,
        VP_BeforeTop,
        VP_Top,
        VP_Middle,
        VP_MiddleDock,
        VP_Bottom,
        VP_AfterBottom
    };

    friend class FwPrimitive;
    friend class FwGeometry;

    FwAnchor(FwPrimitive* primitive);
    virtual ~FwAnchor();

    inline bool isLinked() const;
    void unlink();

    inline HorizontalPolicy horizontalPolicy() const;
    inline void setHorizontalPolicy(HorizontalPolicy policy);

    inline VerticalPolicy verticalPolicy() const;
    inline void setVerticalPolicy(VerticalPolicy policy);

    void setPolicy(HorizontalPolicy hPolicy, VerticalPolicy vPolicy);

    inline FwPrimitive* primitive() const;

    inline FwGeometry* geometry() const;

protected:
    void updateRect(const QRect& parentRect, QRect currentRect);

private:
    FwGeometry* m_geometry;
    HorizontalPolicy m_horizontalPolicy;
    VerticalPolicy m_verticalPolicy;
    FwPrimitive* m_primitive;
};

FwAnchor::HorizontalPolicy FwAnchor::horizontalPolicy() const
{
    return m_horizontalPolicy;
}

void FwAnchor::setHorizontalPolicy(HorizontalPolicy policy)
{
    setPolicy(policy, m_verticalPolicy);
}

FwAnchor::VerticalPolicy FwAnchor::verticalPolicy() const
{
    return m_verticalPolicy;
}

void FwAnchor::setVerticalPolicy(VerticalPolicy policy)
{
    setPolicy(m_horizontalPolicy, policy);
}

bool FwAnchor::isLinked() const
{
    return m_geometry != 0;
}

FwPrimitive* FwAnchor::primitive() const
{
    return m_primitive;
}

FwGeometry* FwAnchor::geometry() const
{
    return m_geometry;
}

///////////////////////////////////////////////////////////////////////////////

class FwGeometry
{
public:
    friend class FwAnchor;

    FwGeometry();
    ~FwGeometry();

    void addAnchor(FwAnchor* anchor);

    inline QRect rect() const;
    inline void setRect(int x, int y, int w, int h);
    void setRect(const QRect& rect);

    inline bool contains(FwAnchor* anchor) const;

    inline QSize size() const;
    inline bool sizeIsNull() const;

    inline QPoint pos() const;

private:  
    QVarLengthArray<FwAnchor*> anchors;
    QRect m_rect;
};

void FwGeometry::setRect(int x, int y, int w, int h)
{
    setRect(QRect(x, y, w, h));
}

QRect FwGeometry::rect() const
{
    return m_rect;
}

bool FwGeometry::contains(FwAnchor* anchor) const
{
    const FwAnchor* const* iterAnchor = anchors.data();
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

#endif //FIREWORKS_ANCHOR_H
