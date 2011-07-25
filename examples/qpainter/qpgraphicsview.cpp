#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>

#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>
#include <QtGui/qapplication.h>

#include "fwcore/fwml.h"

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
    m_view->render(&painter, e->rect());
    painter.setColor(FwColor(0xFF, 0xFF, 0xFF, 0xFF));
    e->accept();

    if(m_view->isSaveFrameEnable() && !m_view->debugLogDir().isEmpty())
    {
        static int imageInc = 0;
        QImage image(size(), QImage::Format_ARGB32);
        foreach(QRect rect, e->region().rects())
        {
            FwPainter painter(QRect(QPoint(0, 0), size()), e->rect(), new QPRender(&image));
            m_view->render(&painter, rect);
        }

        if(!QDir(m_view->debugLogDir()).exists())
        {
            QDir().mkpath(m_view->debugLogDir());
        }

        image.save(QString(m_view->debugLogDir() + "img%1.png").arg(++imageInc));
    }
}

void QPWidget::keyPressEvent(QKeyEvent *e)
{
    FwKeyPressEvent* event = new FwKeyPressEvent(e->key(), e->isAutoRepeat());
    event->setText(e->text());
    QApplication::postEvent(m_view, event);
}

///////////////////////////////////////////////////////////////////////

QPGraphicsView::QPGraphicsView(QObject* parent) :
    BaseClass(parent),
    m_widget(0),
    m_saveFrame(false),
    m_debugLogDir(QString(QCoreApplication::applicationDirPath() + "/debug/"))
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

void QPGraphicsView::invalidateCanvas(const FwRegion& region)
{
    if(m_widget)
    {
        m_widget->update(convert(region));
    }
}

FwVertexArray* QPGraphicsView::createVertexArray() const
{
    return new QPVertexArray();
}

QRegion QPGraphicsView::convert(const FwRegion& region)
{
    QRegion qregion;
    foreach(QRect rect, region.rects())
    {
        qregion = qregion.united(rect);
    }
    return qregion;
}

bool QPGraphicsView::init()
{
    return true;
}

bool QPGraphicsView::loadData(FwMLObject* object)
{
    FwMLBool* debugMode = object->attribute("debugMode")->cast<FwMLBool>();
    if(debugMode)
    {
        setSaveFrame(debugMode->value());
    }
    FwMLString* debugPath = object->attribute("debugDir")->cast<FwMLString>();
    if(debugPath)
    {
        setDebugLogDir(debugPath->value());
    }

    return BaseClass::loadData(object);
}
