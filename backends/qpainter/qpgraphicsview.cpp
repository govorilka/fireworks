#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>

#include "qpgraphicsview.h"

QPWidget::QPWidget(QPGraphicsView* view) :
    BaseClass(),
    m_view(view)
{
}

QPWidget::~QPWidget()
{
}

void QPWidget::resizeEvent(QResizeEvent *e)
{
}

///////////////////////////////////////////////////////////////////////

QPGraphicsView::QPGraphicsView() :
    BaseClass(),
    widget(0)
{
    widget = new QPWidget(this);
    widget->show();
}

QPGraphicsView::~QPGraphicsView()
{
    delete widget;
}

Q_EXPORT_PLUGIN2(backends_qp, QPGraphicsView)
