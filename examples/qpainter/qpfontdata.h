#ifndef FIREWORKS_QPFONTDATA_H
#define FIREWORKS_QPFONTDATA_H

#include <QtGui/qfont.h>

#include "fwtypography/fwfont.h"

#include"fw_backend_qpainter.h"

class FW_BACKEND_QPAINTERSHARED_EXPORT QPFontData : public FwFontData
{
    typedef FwFontData BaseClass;

public:
    QPFontData(const QString& family, int pixelSize);
    virtual ~QPFontData();

    QSize stringSize(const QString& string) const;

    int ascender() const;
    int descender() const;

    int height() const;

    inline QFont qfont() const;

private:
    QFont m_font;
};

QFont QPFontData::qfont() const
{
    return m_font;
}

#endif // FIREWORKS_QPFONTDATA_H
