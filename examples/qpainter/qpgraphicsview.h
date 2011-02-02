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

    QPWidget(QPGraphicsView* view, QWidget* parent = 0);
    virtual ~QPWidget();

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

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

    QPGraphicsView(QObject* parent = 0);
    virtual ~QPGraphicsView();

    QWidget* createWidget(QWidget* parent);

    inline QWidget* widget() const;

protected:

    void updateCanvas(const QRect& rect);

    FwFontData* createFontData(const FwFontDescription& desc);

    FwPixmapData* createBuffer(const FwPixmapDescription& desc);
    FwPixmapData* createBuffer(Fw::BufferMode mode, const QSize& size);
    FwRender* createRender(const QRect& rect);

private:
    QPWidget* m_widget;
};

QWidget* QPGraphicsView::widget() const
{
    return m_widget;
}

#endif // FIREWORKS_QPGRAPHICSVIEW_H
