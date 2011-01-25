#include <QtGui/qfontmetrics.h>

#include "qpfontdata.h"

QPFontData::QPFontData(const QString& family, int pixelSize) :
    BaseClass(),
    m_font(family)
{
    m_font.setPixelSize(pixelSize);
}

QPFontData::~QPFontData()
{
}

QSize QPFontData::stringSize(const QString& string) const
{
    return QFontMetrics(m_font).size(Qt::TextSingleLine, string);
}

int QPFontData::ascender() const
{
    return QFontMetrics(m_font).ascent();
}

int QPFontData::descender() const
{
    return QFontMetrics(m_font).descent();
}

int QPFontData::height() const
{
    return QFontMetrics(m_font).height();
}
