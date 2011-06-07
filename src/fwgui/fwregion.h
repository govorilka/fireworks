#ifndef FIREWORKS_REGION_H
#define FIREWORKS_REGION_H

#include <QtCore/qrect.h>
#include <QtCore/qvector.h>

class FwRegion
{
public:
    FwRegion();

    void addRect(const QRect& rect);

    inline void clear();

    inline bool isEmpty() const;

    inline QVector<QRect> rects() const;

    void validation();

    static bool rectLess(const QRect& r1, const QRect& r2);

private:
    QVector<QRect> m_rects;
};

bool FwRegion::isEmpty() const
{
    return m_rects.isEmpty();
}

void FwRegion::clear()
{
    m_rects.clear();
}

QVector<QRect> FwRegion::rects() const
{
    return m_rects;
}

#endif //FIREWORKS_REGION_H
