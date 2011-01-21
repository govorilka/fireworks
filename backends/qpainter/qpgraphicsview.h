#ifndef FIREWORKS_QPGRAPHICSVIEW_H
#define FIREWORKS_QPGRAPHICSVIEW_H

#include <QtGui/qwidget.h>

#include "fwgui/fwgraphicsview.h"

class QPWidget;
class QPGraphicsView;

class QPWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget BaseClass;

public:
    friend class QPGraphicsView;

    QPWidget(QPGraphicsView* view);
    virtual ~QPWidget();

protected:
    void resizeEvent(QResizeEvent *e);

private:
    QPGraphicsView* m_view;
};

class QPGraphicsView : public FwGraphicsView
{
    Q_OBJECT
    Q_INTERFACES(FwGraphicsView)

    typedef FwGraphicsView BaseClass;

public:
    friend class QPWidget;

    QPGraphicsView();
    virtual ~QPGraphicsView();

private:
    QPWidget* widget;
};

#endif // FIREWORKS_QPGRAPHICSVIEW_H
