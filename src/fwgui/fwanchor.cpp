#include <QtCore/qdebug.h>

#include "fwanchor.h"

#include "fwprimitives/fwprimitive.h"

FwAnchor::FwAnchor(FwPrimitive* primitive) :
    m_geometry(0),
    m_horizontalPolicy(HP_Unchanged),
    m_verticalPolicy(VP_Unchanged),
    m_primitive(primitive)
{
}

FwAnchor::~FwAnchor()
{
    unlink();
}

void FwAnchor::unlink()
{
    if(m_geometry)
    {
        Q_ASSERT(m_geometry->contains(this));
        QVarLengthArray<FwAnchor*> newAnchors;
        FwAnchor** iterAnchor = m_geometry->anchors.data();
        for(int i = 0; i < m_geometry->anchors.size(); i++, iterAnchor++)
        {
            if((*iterAnchor) != this)
            {
                newAnchors.append(*iterAnchor);
            }
        }
        m_geometry->anchors = newAnchors;
        m_geometry = 0;
    }
}

void FwAnchor::updateRect(const QRect& parentRect, QRect currentRect)
{
    switch(m_horizontalPolicy)
    {
    case HP_Unchanged:
        currentRect.setLeft(parentRect.x() + m_primitive->m_x);
        break;

    case HP_BeforeLeft:
        currentRect.moveLeft(parentRect.x() - currentRect.width() + m_primitive->m_x);
        break;

    case HP_Left:
        currentRect.moveLeft(parentRect.x() + m_primitive->m_x);
        break;

    case HP_Center:
        currentRect.moveLeft(parentRect.x() + (parentRect.width() - currentRect.width()) * 0.5 + m_primitive->m_x);
        break;

    case HP_CenterDock:
        currentRect.moveLeft(parentRect.x() + m_primitive->m_x);
        currentRect.setWidth(parentRect.width());
        break;

    case HP_Right:
        currentRect.moveLeft(parentRect.right() + 1 - currentRect.width() + m_primitive->m_x);
        break;

    case HP_AfterRight:
        currentRect.moveLeft(parentRect.right() + m_primitive->m_x);
        break;

    default:
        break;
    }

    switch(m_verticalPolicy)
    {
    case VP_Unchanged:
        currentRect.setTop(parentRect.y() + m_primitive->m_y);
        break;

    case VP_BeforeTop:
        currentRect.moveTop(parentRect.y() - currentRect.height() + m_primitive->m_y);
        break;

    case VP_Top:
        currentRect.moveTop(parentRect.y()+ m_primitive->m_y);
        break;

    case VP_Middle:
        currentRect.moveTop(parentRect.y() + (parentRect.height() - currentRect.height()) * 0.5 + m_primitive->m_y);
        break;

    case VP_MiddleDock:
        currentRect.moveTop(parentRect.y()+ m_primitive->m_y);
        currentRect.setHeight(parentRect.height());
        break;

    case VP_Bottom:
        currentRect.moveTop(parentRect.bottom() - currentRect.height() + m_primitive->m_y);
        break;

    case VP_AfterBottom:
        currentRect.moveTop(parentRect.bottom() + m_primitive->m_y);
        break;

    default:
        break;
    }

    m_primitive->setGeometryRect(currentRect);
}

void FwAnchor::setPolicy(HorizontalPolicy hPolicy, VerticalPolicy vPolicy)
{
    if(m_horizontalPolicy != hPolicy || m_verticalPolicy != vPolicy)
    {
        m_horizontalPolicy = hPolicy;
        m_verticalPolicy = vPolicy;
        if(m_geometry)
        {
            updateRect(m_geometry->rect(), m_primitive->rect());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

FwGeometry::FwGeometry() :
   m_rect(0, 0, 0, 0)
{
}

FwGeometry::~FwGeometry()
{
    FwAnchor** iterAnchor = anchors.data();
    for(int i = 0; i < anchors.size(); i++, iterAnchor++)
    {
        (*iterAnchor)->m_geometry = 0;
    }
}

void FwGeometry::addAnchor(FwAnchor* anchor)
{
    if(anchor->m_geometry != this)
    {
        if(anchor->m_geometry)
        {
            anchor->unlink();
        }
        anchors.append(anchor);
        anchor->m_geometry = this;
        anchor->updateRect(m_rect, anchor->m_primitive->rect());
    }
}

void FwGeometry::setRect(const QRect& rect)
{
    if(m_rect != rect)
    {
        m_rect = rect;
        FwAnchor** iterAnchor = anchors.data();
        for(int i = 0; i < anchors.size(); i++, iterAnchor++)
        {
            (*iterAnchor)->updateRect(rect,
                                      (*iterAnchor)->m_primitive->rect());
        }
    }
}
