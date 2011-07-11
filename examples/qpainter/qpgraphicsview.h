#ifndef FIREWORKS_QPGRAPHICSVIEW_H
#define FIREWORKS_QPGRAPHICSVIEW_H

#include <QtGui/qwidget.h>

#include "fwgui/fwgraphicsview.h"

#include"fw_backend_qpainter.h"

class QPWidget;
class QPGraphicsView;

class FW_BACKEND_QPAINTERSHARED_EXPORT QPWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget BaseClass;

public:
    friend class QPGraphicsView;

    QPWidget(QPGraphicsView* view, QWidget* parent = 0);
    virtual ~QPWidget();

    inline bool isSaveFrameEnable() const;
    inline void setSaveFrame(bool enable);

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    QPGraphicsView* m_view;
    bool m_saveFrame;
};

bool QPWidget::isSaveFrameEnable() const
{
    return m_saveFrame;
}

void QPWidget::setSaveFrame(bool enable)
{
    m_saveFrame = enable;
}

///////////////////////////////////////////////////////////////////////////////

class FW_BACKEND_QPAINTERSHARED_EXPORT QPGraphicsView : public FwGraphicsView
{
    Q_OBJECT
    typedef FwGraphicsView BaseClass;

public:
    friend class QPWidget;

    QPGraphicsView(QObject* parent = 0);
    virtual ~QPGraphicsView();

    QWidget* createWidget(QWidget* parent);

    inline QWidget* widget() const;

    FwVertexArray* createVertexArray() const;

    static QRegion convert(const FwRegion& region);

    bool init();

protected:

    void invalidateCanvas(const FwRegion& region);

    FwFontData* createFontData(const FwFontDescription& desc);

    FwPixmapData* createBuffer(const FwPixmapDescription& desc);
    FwPixmapData* createBuffer(Fw::BufferMode mode, const QSize& size);

private:
    QPWidget* m_widget;
};

QWidget* QPGraphicsView::widget() const
{
    return m_widget;
}

#endif // FIREWORKS_QPGRAPHICSVIEW_H
