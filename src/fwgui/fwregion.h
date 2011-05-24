#ifndef FIREWORKS_REGION_H
#define FIREWORKS_REGION_H

#include <QtCore/qrect.h>
#include <QtCore/qlist.h>

class FwRegion
{
public:
    FwRegion();

    inline void addRect(const QRect& rect);

    inline void clear();

    inline bool isEmpty() const;

    inline QList<QRect> rects() const;

private:
    QList<QRect> m_rects;
};

void FwRegion::addRect(const QRect& rect)
{
    m_rects.append(rect);
}

bool FwRegion::isEmpty() const
{
    return m_rects.isEmpty();
}

void FwRegion::clear()
{
    m_rects.clear();
}

QList<QRect> FwRegion::rects() const
{
    return m_rects;
}

#endif //FIREWORKS_REGION_H
