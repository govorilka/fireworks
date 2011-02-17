#ifndef FIREWORKS_FONT_H
#define FIREWORKS_FONT_H

#include <QtCore/qsharedpointer.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qsize.h>

class FwMLNode;

class FwFontDescription
{
public:
    FwFontDescription();
    FwFontDescription(const QString& name, int pixcelSize);

    inline QString family() const;
    inline int pixcelSize() const;

    QString key() const;

    QString fileName() const;

    inline bool operator !=(const FwFontDescription& other) const;

    bool apply(FwMLNode* node);

    inline bool isNull() const;

private:
    QString m_family;
    int m_pixcelSize;
};

QString FwFontDescription::family() const
{
    return m_family;
}

int FwFontDescription::pixcelSize() const
{
    return m_pixcelSize;
}

bool FwFontDescription::operator !=(const FwFontDescription& other) const
{
    return m_family != other.m_family ||
           m_pixcelSize != other.m_pixcelSize;
}

bool FwFontDescription::isNull() const
{
    return m_family.isEmpty() || m_pixcelSize == 0;
}

////////////////////////////////////////////////////////////////////////////////

class FwFontData
{
public:
    FwFontData();
    virtual ~FwFontData();

    virtual void release();

    virtual QSize stringSize(const QString& string) const = 0;

    virtual int ascender() const = 0;
    virtual int descender() const = 0;

    virtual int height() const = 0;
};

////////////////////////////////////////////////////////////////////////////////

class FwFont : protected QSharedPointer<FwFontData>
{
    typedef QSharedPointer<FwFontData> BaseClass;

public:
    FwFont();
    FwFont(FwFontData* data);
    ~FwFont();

    void release();

    bool isNull() const;

    bool operator==(const FwFont &other) const;
    bool operator!=(const FwFont &other) const;

    inline FwFontData* data() const;

    QSize stringSize(const QString& string) const;
    inline int stringWidth(const QString& string) const;
    inline int stringHeight(const QString& string) const;

    int ascender() const;
    int descender() const;

    int height() const;
};

Q_DECLARE_METATYPE(FwFont);

FwFontData* FwFont::data() const
{
    return const_cast<FwFont*>(this)->BaseClass::data();
}

int FwFont::stringWidth(const QString& string) const
{
    return stringSize(string).width();
}

int FwFont::stringHeight(const QString& string) const
{
    return stringSize(string).height();
}

#endif //FONT_H
