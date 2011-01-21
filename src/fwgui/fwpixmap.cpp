#include <QtCore/qrect.h>

#include "fwpixmap.h"

#include "fwcore/fwml.h"

#include "fwgui/fwrender.h"
#include "fwgui/fwcanvas.h"

FwPixmapDescription::FwPixmapDescription() :
    m_source(QString()),
    m_blending(true)
{

}

FwPixmapDescription::FwPixmapDescription(const QString& name, bool blending) :
    m_source(name),
    m_blending(blending)
{
}

bool FwPixmapDescription::apply(FwMLNode* node)
{
    FwMLString* name = node->cast<FwMLString>();
    if(name)
    {
        m_source = QString::fromUtf8(name->value());
        m_blending = true;
        return true;
    }
    else
    {
        FwMLObject* object = node->cast<FwMLObject>();
        if(object)
        {
            FwMLString* sourceNode = object->attribute("source")->cast<FwMLString>();
            if(sourceNode)
            {
                QString source = QString::fromUtf8(sourceNode->value());
                if(!source.isEmpty())
                {
                    FwMLNode* blendingNode = object->attribute("blending");
                    if(blendingNode)
                    {
                        bool bOk = false;
                        bool blending = blendingNode->toBool(&bOk);
                        if(bOk)
                        {
                            m_blending = blending;
                            m_source = source;
                            return true;
                        }
                    }
                    else
                    {
                        m_source = source;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

FwPixmapData::FwPixmapData(const QString& fileName) :
    m_fileName(fileName),
    blend(false)
{
}

FwPixmapData::~FwPixmapData()
{
}

void FwPixmapData::setBlendingEnable(bool enable)
{
    blend = enable;
}

////////////////////////////////////////////////////////////////////////////////

FwPixmap::FwPixmap()
    : BaseClass()
{
}

FwPixmap::FwPixmap(FwPixmapData* data)
    : BaseClass(data)
{
}

FwPixmap::~FwPixmap()
{
}

void FwPixmap::release()
{
    FwPixmapData* data = this->data();
    if(data)
    {
        data->release();
    }
}

bool FwPixmap::isNull() const
{
    return data() == 0;
}

bool FwPixmap::operator==(const FwPixmap &other) const
{
    return data() == other.data();
}

bool FwPixmap::operator!=(const FwPixmap &other) const
{
    return data() != other.data();
}

QSize FwPixmap::size() const
{
    FwPixmapData* data = this->data();
    if(data)
    {
        return data->size();
    }
    return QSize(0, 0);
}

FwPixmap FwPixmap::resized(const QSize& size, ResizeMode mode) const
{
    if(size == this->size())
    {
        return *this;
    }

    FwPixmapData* data = this->data();
    if(data)
    {
        FwPixmapData* newData = 0;
        switch(mode)
        {
        case RM_None:
            newData = data->resized(size);
            break;
        case RM_Scale:
            newData = data->scaled(size);
            break;
        case RM_Tile:
            newData = data->tiled(size);
            break;
        }

        if(newData)
        {
            FwPixmap pixmap(newData);
            pixmap.setBlendEnabled(data->blend);
            return pixmap;
        }
    }
    return FwPixmap();
}

/*!
Функция возвращает копию текущей картинки с вераззной прямоугольной областью,
заданной параметром rect.
\param rect Прямоугольная область, которая будет вырезана на результирующей
картинке
*/
FwPixmap FwPixmap::chromaKey(const QRect& rect) const
{
    FwPixmapData* data = this->data();
    if(data)
    {
        QRect r = rect.normalized();
        FwPixmapData* newData = data->chromaKey(r.x(), r.y(), r.width(), r.height());
        if(newData)
        {
            return FwPixmap(newData);
        }
    }
    return FwPixmap();
}

/*!
Возращает значение флага blend. Если функци вернулf true, то при рисование
картинки на экране (используя функции Canvas), цвета картинки будут смешиваться
с цветами заднего фона. Если false. Картинка будет рисоваться по верх.
\note По умолчанию, значение флага blend равно true
\sa void FwPixmap::setBlendEnabled(bool enable)
*/
bool FwPixmap::isBlendEnabled() const
{
    FwPixmapData* data = this->data();
    if(data)
    {
        return data->blend;
    }
    return false;
}

/*!
Устанавливает необходимость смешиваания цветов, при отображение картинки на
экране.
\param enable Если значение true, то рисование картинки через Canvas, цвета
картинки будут смешиваться с цветами фона, на котором отображается картинка.
\sa bool FwPixmap::isBlendEnabled() const
*/
void FwPixmap::setBlendEnabled(bool enable)
{
    FwPixmapData* data = this->data();
    if(data)
    {
        data->blend = enable;
    }
}

FwCanvas* FwPixmap::createCanvas() const
{
    FwPixmapData* data = this->data();
    if(data)
    {
        FwRender* render = data->createRender();
        if(render)
        {
            return new FwCanvas(render);
        }
    }
    return 0;
}
