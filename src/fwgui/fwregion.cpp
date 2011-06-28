#include "fwregion.h"

FwRegion::FwRegion() :
    m_objectRect()
{
}

void FwRegion::addRect(const QRect& rect)
{
    if(rect.width() > 0 && rect.height() > 0)
    {
        if(m_rects.isEmpty())
        {
            m_rects.append(rect);
        }
        else
        {
            int deltaLeft = 0, deltaRight = 0, deltaTop = 0, deltaBottom = 0;

            QRect curr = rect;

            QVector<QRect>::iterator nextIter = m_rects.begin();
            QVector<QRect>::iterator endIter = m_rects.end();
            while(nextIter != endIter)
            {
                QRect& next = *nextIter;
                                if(!next.width() || !next.height())
                {
                    ++nextIter;
                    continue;
                }

                if(((deltaLeft = curr.left() - next.right()) > 1) ||
                   ((deltaRight =  next.left() - curr.right()) > 1) ||
                   ((deltaTop = curr.top() - next.bottom()) > 1) ||
                   ((deltaBottom = next.top() - curr.bottom()) > 1))
                {
                    ++nextIter;
                    continue;
                }

                int choiseX = curr.y() < next.y();
                if(curr.bottom() > next.bottom())
                {
                    choiseX += 2;
                }
                int choiseY = curr.left() < next.left();
                if(curr.right() > next.right())
                {
                    choiseY += 2;
                }

                switch(choiseX)
                {
                case 0:
                    {
                        switch(choiseY)
                        {
                        case 0:
                            return;

                        case 1:
                            if(deltaTop == deltaBottom)
                            {
                                curr.setRight(next.right());
                                next = QRect();
                            }
                            else
                            {
                                curr.setRight(next.left() - 1);
                            }
                            break;

                        case 2:
                            if(deltaTop == deltaBottom)
                            {
                                curr.setLeft(next.left());
                                next = QRect();
                            }
                            else
                            {
                                curr.setLeft(next.right() + 1);
                            }
                            break;

                        case 3:
                            {
                                int right = next.left() - 1;
                                addRect(curr.adjusted(next.right() - curr.left() + 1, 0, 0, 0));
                                curr.setRight(right);
                            }
                            break;
                        }
                    }
                    break;

                case 1:
                    {
                        switch(choiseY)
                        {
                        case 0:
                            if(deltaLeft == deltaRight)
                            {
                                curr.setBottom(next.bottom());
                                next = QRect();
                            }
                            else
                            {
                                curr.setBottom(next.top() - 1);
                            }
                            break;

                        case 1:
                            {
                                int left = next.left() - 1;
                                addRect(curr.adjusted(next.left() - curr.left(), 0, 0, curr.bottom() - next.top() -1));
                                curr.setRight(left);
                            }
                            break;

                        case 2:
                            {
                                int right = next.right() + 1;
                                addRect(curr.adjusted(0, 0, next.right() - curr.right(), next.top() - curr.bottom() - 1));
                                curr.setLeft(right);
                            }
                            break;

                        case 3:
                            next.setTop(curr.bottom() + 1);
                            break;
                        }
                    }
                    break;

                case 2:
                    {
                        switch(choiseY)
                        {
                        case 0:
                            if(deltaLeft == deltaRight)
                            {
                                curr.setTop(next.top());
                                next = QRect();
                            }
                            else
                            {
                                return;
                            }
                            break;

                        case 1:
                            {
                                QRect intersectRect = next.adjusted(curr.right() - next.left() + 1, curr.top() - next.top(), 0, 0);
                                next.setBottom(curr.top() - 1);
                                if(intersectRect.width() && intersectRect.height())
                                {
                                    m_rects.append(intersectRect);
                                }
                            }
                            break;

                        case 2:
                            {
                                QRect intersectRect = next.adjusted(0, curr.top() - next.top(), curr.left() - next.right() - 1, 0);
                                next.setBottom(curr.top() - 1);
                                if(intersectRect.width() && intersectRect.height())
                                {
                                    m_rects.append(intersectRect);
                                }
                            }
                            break;

                        case 3:
                            next.setBottom(curr.top() - 1);
                            break;
                        }
                    }
                    break;

                case 3:
                    {
                        switch(choiseY)
                        {
                        case 0:
                            {
                                QRect intersectRect = next.adjusted(curr.right() - next.left() + 1, 0, 0, 0);
                                next.setRight(curr.left() - 1);
                                if(intersectRect.width() && intersectRect.height())
                                {
                                    m_rects.append(intersectRect);
                                }
                            }
                            break;

                        case 1:
                            next.setLeft(curr.right() + 1);
                            break;

                        case 2:
                            next.setRight(curr.left() - 1);
                            break;

                        case 3:
                            next = QRect();
                            break;
                        }
                    }
                    break;
                }
                ++nextIter;
            }

            if(curr.width() && curr.height())
            {
                m_rects.append(curr);
            }

        }
    }
}

void FwRegion::validation()
{
    QVector<QRect> rects;
    foreach(QRect rect, m_rects)
    {
        if(rect.width() && rect.height())
        {
            rects.append(rect);
        }
    }
    m_rects = rects;
}

bool FwRegion::rectLess(const QRect& r1, const QRect& r2)
{
    if(r1.x() < r2.x())
    {
        return true;
    }
    return r1.x() == r2.x() ? r1.y() < r2.y() : false;
}

