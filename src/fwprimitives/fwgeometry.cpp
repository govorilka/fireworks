#include "fwgeometry.h"
#include "fwprimitive.h"

FwGeometry::FwGeometry() :
   m_rect(0, 0, 0, 0),
   m_posChanged(false),
   m_sizeChanged(false)
{
}

FwGeometry::~FwGeometry()
{
    FwPrimitive** iterAnchor = anchors.data();
    for(int i = 0; i < anchors.size(); i++, iterAnchor++)
    {
        (*iterAnchor)->m_parentGeometry = 0;
    }
}

void FwGeometry::setRect(const QRect& rect)
{
    if(m_rect != rect)
    {
        m_posChanged = (m_rect.topLeft() != rect.topLeft());
        m_sizeChanged = (m_rect.size() != rect.size());
        m_rect = rect;
    }
}

void FwGeometry::apply()
{
    m_posChanged = false;
    m_sizeChanged = false;
    FwPrimitive** iterAnchor = anchors.data();
    for(int i = 0; i < anchors.size(); i++, iterAnchor++)
    {
        (*iterAnchor)->updateGeometryRect(m_rect, (*iterAnchor)->rect());
    }
}
