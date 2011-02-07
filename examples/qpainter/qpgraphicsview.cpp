#include <QtCore/qdebug.h>

#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>
#include <QtGui/qapplication.h>

#include "fwgui/fwpainter.h"
#include "fwgui/fwguievent.h"

#include "qpgraphicsview.h"
#include "qpfontdata.h"
#include "qprender.h"
#include "qppixmapdata.h"

QPWidget::QPWidget(QPGraphicsView* view, QWidget* parent) :
    BaseClass(parent),
    m_view(view)
{
}

QPWidget::~QPWidget()
{
    if(m_view)
    {
        m_view->m_widget = 0;
    }
}

void QPWidget::resizeEvent(QResizeEvent *e)
{
    m_view->setSize(e->size());
    e->accept();
}

void QPWidget::paintEvent(QPaintEvent *e)
{
    FwPainter painter(QRect(QPoint(0, 0), size()), e->rect(), new QPRender(this));
    m_view->render(&painter);
    e->accept();
}

void QPWidget::keyPressEvent(QKeyEvent *e)
{
    QApplication::postEvent(m_view, new FwKeyPressEvent(e->key(), e->isAutoRepeat()));
}

///////////////////////////////////////////////////////////////////////

QPGraphicsView::QPGraphicsView(QObject* parent) :
    BaseClass(parent),
    m_widget(0)
{
}

QPGraphicsView::~QPGraphicsView()
{
    if(m_widget)
    {
        m_widget->m_view = 0;
        delete m_widget;
    }
}

QWidget* QPGraphicsView::createWidget(QWidget* parent)
{
    if(!m_widget)
    {
        m_widget = new QPWidget(this, parent);
    }
    return m_widget;
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

void QPGraphicsView::updateCanvas(const QRect& rect)
{
    if(m_widget)
    {
        m_widget->update(rect);
    }
}
