#ifndef FIREWORKS_PIXMAP_H
#define FIREWORKS_PIXMAP_H

#include <QtCore/qsharedpointer.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qsize.h>

#include "fireworks.h"

class FwPixmap;
class FwPixmapDescription;
class FwRender;
class FwMLNode;

class FIREWORKSSHARED_EXPORT FwPixmapDescription
{
public:

    FwPixmapDescription();
    FwPixmapDescription(const QString& source, bool blending);

    inline QString source() const;
    inline void setSource(const QString& source);

    inline bool blending() const;
    inline void setBlending(bool blending);

    inline bool operator !=(const FwPixmapDescription& other) const;

    inline bool isNull() const;

    inline QString key() const;

    bool apply(FwMLNode* node);

private:
    QString m_source;
    bool m_blending;
};

QString FwPixmapDescription::source() const
{
    return m_source;
}

void FwPixmapDescription::setSource(const QString& source)
{
    m_source = source;
}

bool FwPixmapDescription::blending() const
{
    return m_blending;
}

void FwPixmapDescription::setBlending(bool blending)
{
    m_blending = blending;
}

bool FwPixmapDescription::operator !=(const FwPixmapDescription& other) const
{
    return m_blending != other.m_blending || m_source != other.m_source;
}

bool FwPixmapDescription::isNull() const
{
    return m_source.isEmpty();
}

QString FwPixmapDescription::key() const
{
    return QString("%1,%2").arg(m_source).arg(m_blending);
}

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPixmapData
{
public:
    friend class FwPixmap;

    explicit FwPixmapData(const QString& fileName);
    virtual ~FwPixmapData();

    inline QString fileName() const;

    virtual void release() = 0;

    virtual QSize size() const = 0;

    virtual FwPixmapData* tiled(const QSize& size) const = 0;
    virtual FwPixmapData* resized(const QSize& size) const = 0;
    virtual FwPixmapData* scaled(const QSize& size) const = 0;

    inline bool isBlendEnabled() const;
    void setBlendingEnable(bool enable);

    virtual FwRender* createRender() = 0;

private:
    bool blend;
    QString m_fileName;
};

QString FwPixmapData::fileName() const
{
    return m_fileName;
}

bool FwPixmapData::isBlendEnabled() const
{
    return blend;
}

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPixmap : protected QSharedPointer<FwPixmapData>
{
    typedef QSharedPointer<FwPixmapData> BaseClass;

public:

    enum ResizeMode
    {
        RM_None,
        RM_Scale,
        RM_Tile,
    };

    FwPixmap();
    explicit FwPixmap(FwPixmapData* data);
    ~FwPixmap();

    void release();

    bool isNull() const;

    bool operator==(const FwPixmap &other) const;
    bool operator!=(const FwPixmap &other) const;

    inline FwPixmapData* surface() const;

    QSize size() const;
    inline int width() const;
    inline int height() const;

    inline FwPixmap tiled(int w, int h) const;
    inline FwPixmap scaled(int w, int h) const;
    inline FwPixmap resized(int w, int h) const;

    inline FwPixmap tiled(const QSize& size) const;
    inline FwPixmap scaled(const QSize& size) const;
    FwPixmap resized(const QSize& size, ResizeMode mode = RM_None) const;

    inline void clear();

    bool isBlendEnabled() const;
    void setBlendEnabled(bool enable);
};

Q_DECLARE_METATYPE(FwPixmap);

FwPixmapData* FwPixmap::surface() const
{
    return const_cast<FwPixmap*>(this)->BaseClass::data();
}

int FwPixmap::width() const
{
    return size().width();
}

int FwPixmap::height() const
{
    return size().height();
}

FwPixmap FwPixmap::tiled(int w, int h) const
{
    return tiled(QSize(w, h));
}

FwPixmap FwPixmap::scaled(int w, int h) const
{
    return scaled(QSize(w, h));
}

FwPixmap FwPixmap::resized(int w, int h) const
{
    return resized(QSize(w, h), RM_None);
}

FwPixmap FwPixmap::tiled(const QSize& size) const
{
    return resized(size, RM_Tile);
}

FwPixmap FwPixmap::scaled(const QSize& size) const
{
    return resized(size, RM_Scale);
}

void FwPixmap::clear()
{
    *this = FwPixmap();
}

#endif // FIREWORKS_PIXMAP_H
