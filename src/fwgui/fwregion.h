#ifndef FIREWORKS_REGION_H
#define FIREWORKS_REGION_H

#include <QtCore/qrect.h>
#include <QtCore/qvector.h>
#include <QtCore/qstack.h>

#include "fireworks.h"

class FIREWORKSSHARED_EXPORT FwRegion
{
public:
    FwRegion();

    void addRect(const QRect& rect);
    inline void addChildrenRect(const QRect& rect);

    void clear();

    inline bool isEmpty() const;

    inline QVector<QRect> rects() const;

    void validation();

    static bool rectLess(const QRect& r1, const QRect& r2);

    inline QRect objectRect() const;

    inline void pushObjectRect(const QRect& rect);
    inline void popObjectRect();

private:
    QVector<QRect> m_rects;

    QRect m_sceneRect;

    QRect m_objectRect;
    QStack<QRect> m_objectRectStack;
};

bool FwRegion::isEmpty() const
{
    return m_rects.isEmpty();
}

//void FwRegion::clear()
//{
//    m_rects.clear();
//}

QVector<QRect> FwRegion::rects() const
{
    return m_rects;
}

QRect FwRegion::objectRect() const
{
    return m_objectRect;
}

void FwRegion::pushObjectRect(const QRect& rect)
{
    m_objectRectStack.push(m_objectRect);
    m_objectRect = rect;
}

void FwRegion::popObjectRect()
{
    m_objectRect = (m_objectRectStack.isEmpty() ? QRect() : m_objectRectStack.pop());
}

void FwRegion::addChildrenRect(const QRect& rect)
{
    addRect(m_objectRect.intersected(rect));
}

#endif //FIREWORKS_REGION_H
