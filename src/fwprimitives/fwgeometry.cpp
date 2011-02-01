#include "fwgeometry.h"
#include "fwprimitive.h"

FwGeometry::FwGeometry() :
   m_rect(0, 0, 0, 0)
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
        m_rect = rect;
        FwPrimitive** iterAnchor = anchors.data();
        for(int i = 0; i < anchors.size(); i++, iterAnchor++)
        {
            (*iterAnchor)->updateGeometryRect(rect, (*iterAnchor)->rect());
        }
    }
}
