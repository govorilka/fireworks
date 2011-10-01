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



protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    QPGraphicsView* m_view;

};



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

    inline bool isSaveFrameEnable() const;
    inline void setSaveFrame(bool enable);

    inline QString debugLogDir() const;
    inline void setDebugLogDir(const QString& dir);

    bool loadData(FwMLObject* object);

protected:

    void invalidateCanvas(const FwRegion& region);

    FwFontData* createFontData(const FwFontDescription& desc);

    FwPixmapData* createBuffer(const FwPixmapDescription& desc);
    FwPixmapData* createBuffer(Fw::BufferMode mode, const QSize& size);

private:
    QPWidget* m_widget;
    bool m_saveFrame;
    QString m_debugLogDir;
};

QWidget* QPGraphicsView::widget() const
{
    return m_widget;
}

bool QPGraphicsView::isSaveFrameEnable() const
{
    return m_saveFrame;
}

void QPGraphicsView::setSaveFrame(bool enable)
{
    m_saveFrame = enable;
}

QString QPGraphicsView::debugLogDir() const
{
    return m_debugLogDir;
}

void QPGraphicsView::setDebugLogDir(const QString& dir)
{
    m_debugLogDir = dir;
}

#endif // FIREWORKS_QPGRAPHICSVIEW_H
