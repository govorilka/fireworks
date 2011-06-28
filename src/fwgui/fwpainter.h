#ifndef FIREWORKS_PAINTER_H
#define FIREWORKS_PAINTER_H

#include "fwcore/fwcolor.h"

#include "fwgui/fwrender.h"
#include "fwgui/fwpixmap.h"

#include "fwtypography/fwfont.h"

class FIREWORKSSHARED_EXPORT FwPainter
{
public:
    FwPainter(const QRect& window, const QRect& clipRect, FwRender* render);
    virtual ~FwPainter();

    inline int nullX() const;
    inline int nullY() const;

    inline QRect window() const;

    inline QRect clipRect() const;
    inline QRect setClipRect(const QRect& rect);
    inline void restoreClipRect(const QRect& rect);

    inline FwColor color() const;
    inline void setColor(quint8 a, quint8 r, quint8 g, quint8 b);
    inline void setColor(const FwColor& color);

    inline float opacity() const;
    inline void setOpacity(float opacity);

    inline FwFont font() const;
    inline void setFont(const FwFont& font);

    inline void drawFillRect(const QRect& rect);
    inline void drawRect(const QRect& rect);
    inline void drawLine(const QLine& line);
    inline void drawPixmap(int x, int y, const FwPixmap& pixmap);
    inline void drawPixmap(const QRect& rect, const FwPixmap& pixmap, const QRect* srcRect = 0);
    inline void drawString(int x, int y, const QByteArray& utf8String);
    inline void drawBuffer(const QRect& rect, FwPixmapData* pixmap, const QRect srcRect);
    inline void drawVertexArray(FwVertexArray* array);

private:
    FwRender* m_render;
    int m_nullX;
    int m_nullY;
    QSize m_windowSize;
    QRect m_clipRect;
    FwColor m_color;
    float m_opacity;
    FwFont m_font;
};

#include "fwgui/fwpainter_inl.h"

#endif // FIREWORKS_PAINTER_H
