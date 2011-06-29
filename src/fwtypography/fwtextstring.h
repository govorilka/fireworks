#ifndef FIREWORKS_TEXTSTRING_H
#define FIREWORKS_TEXTSTRING_H

#include <QtCore/qstring.h>
#include <QtCore/qpoint.h>
#include <QtCore/qbytearray.h>

class FwJustification;

class FwTextString
{
public:
    friend class FwJustification;

    FwTextString();

    inline QPoint pos() const;
    void setPos(const QPoint& pos);

    inline int x() const;
    inline int y() const;

    inline QString value() const;

    inline QByteArray utf8() const;
    inline int width() const;

    inline bool isEmpty() const;

    inline bool isEndBreak() const;

private:
    QPoint m_pos;
    QString m_value;
    QByteArray m_utf8;
    int m_width;
    bool m_endBreak;
};

QPoint FwTextString::pos() const
{
    return m_pos;
}

QString FwTextString::value() const
{
    return m_value;
}

QByteArray FwTextString::utf8() const
{
    return m_utf8;
}

int FwTextString::x() const
{
    return m_pos.x();
}

int FwTextString::y() const
{
    return m_pos.y();
}

bool FwTextString::isEmpty() const
{
    return m_value.isEmpty();
}

int FwTextString::width() const
{
    return m_width;
}

bool FwTextString::isEndBreak() const
{
    return m_endBreak;
}
#endif // FWTEXTSTRING_H
