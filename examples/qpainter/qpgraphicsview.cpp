#include <QtCore/qdebug.h>

#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>

#include "fwgui/fwcanvas.h"

#include "qpgraphicsview.h"
#include "qpfontdata.h"
#include "qprender.h"
#include "qppixmapdata.h"

QPWidget::QPWidget(QPGraphicsView* view) :
    BaseClass(),
    m_view(view)
{
}

QPWidget::~QPWidget()
{
    if(m_view)
    {
        m_view->widget = 0;
    }
}

void QPWidget::resizeEvent(QResizeEvent *e)
{
    m_view->setSize(e->size());
    e->accept();
}

void QPWidget::paintEvent(QPaintEvent *e)
{
    FwCanvas canvas(new QPRender(this));
    canvas.setRect(e->rect());
    m_view->invalidateView(e->rect(), &canvas);
    e->accept();
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
    if(widget)
    {
        widget->m_view = 0;
        delete widget;
    }
}

FwFontData* QPGraphicsView::createFontData(const FwFontDescription& desc)
{
    return new QPFontData(desc.family(), desc.pixcelSize());
}

FwPixmapData* QPGraphicsView::createBuffer(const FwPixmapDescription& desc)
{
    QPixmap pixmap;
    if(pixmap.load(desc.source()))
    {
        return new QPPixmapData(pixmap, desc.source());
    }

    return 0;
}

FwPixmapData* QPGraphicsView::createBuffer(Fw::BufferMode mode, const QSize& size)
{
    Q_UNUSED(mode);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    return new QPPixmapData(pixmap, "");
}

FwRender* QPGraphicsView::createRender(const QRect& rect)
{
    return 0;
}

void QPGraphicsView::bufferFlip(const QRegion& region)
{
}

void QPGraphicsView::clearBackground()
{
}

Q_EXPORT_PLUGIN2(backends_qp, QPGraphicsView)
