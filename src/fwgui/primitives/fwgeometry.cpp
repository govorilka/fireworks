#include "fwgeometry.h"
#include "fwprimitive.h"

#include "fwcore/fwml.h"

FwMargin::FwMargin() :
    left(0),
    right(0),
    top(0),
    bottom(0)
{
}

bool FwMargin::apply(FwMLObject* object)
{
    bool bOk = false;
    int mlLeft = left;
    int mlRight = right;
    int mlTop = top;
    int mlBottom = bottom;

    FwMLNode* leftNode = object->attribute("left");
    if(leftNode)
    {
        mlLeft = leftNode->toInt(&bOk);
        if(!bOk)
        {
            return false;
        }
    }

    FwMLNode* rightNode = object->attribute("right");
    if(rightNode)
    {
        mlRight = rightNode->toInt(&bOk);
        if(!bOk)
        {
            return false;
        }
    }

    FwMLNode* topNode = object->attribute("top");
    if(topNode)
    {
        mlTop = topNode->toInt(&bOk);
        if(!bOk)
        {
            return false;
        }
    }

    FwMLNode* bottomNode = object->attribute("bottom");
    if(bottomNode)
    {
        mlBottom = bottomNode->toInt(&bOk);
        if(!bOk)
        {
            return false;
        }
    }

    left = mlLeft;
    right = mlRight;
    bottom = mlBottom;
    top = mlTop;

    return true;
}

//////////////////////////////////////////////////////////////////////

FwGeometry::FwGeometry() :
    m_rect(0, 0, 0, 0),
    m_contentRect(0, 0, 0, 0),
    m_oldRect(0, 0, 0, 0)
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
        m_contentRect = m_margin.contentRect(m_rect);
    }
}

void FwGeometry::updateChildrenRect()
{
    FwPrimitive** iterAnchor = anchors.data();
    for(int i = 0; i < anchors.size(); i++, iterAnchor++)
    {
        (*iterAnchor)->updateGeometryRect();
    }
}

void FwGeometry::unlink(FwPrimitive* primitive)
{
    Q_ASSERT(contains(primitive));

    int array_size = anchors.size();
    if(array_size > 1)
    {
        QVarLengthArray<FwPrimitive*> newAnchors(array_size - 1);
        FwPrimitive** iterAnchor = anchors.data();
        for(int i = 0, j = 0; i < array_size; i++, iterAnchor++)
        {
            if((*iterAnchor) != primitive)
            {
                newAnchors[j] = (*iterAnchor);
                j++;
            }
        }
        anchors = newAnchors;
    }
    else
    {
        anchors.clear();
    }
}


