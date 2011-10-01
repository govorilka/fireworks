#include "fwfont.h"

#include "fwcore/fwml.h"

////////////////////////////////////////////////////////////////////////////////

FwFontDescription::FwFontDescription() :
    m_pixcelSize()
{
}

FwFontDescription::FwFontDescription(const QString& family, int pixcelSize) :
    m_family(family),
    m_pixcelSize(pixcelSize)
{
}

QString FwFontDescription::key() const
{
    return QString("%1,%2")
            .arg(m_family)
            .arg(m_pixcelSize);
}

QString FwFontDescription::fileName() const
{
    return QString("./resources/fonts/%1.ttf").arg(m_family.simplified().replace(" ", "_"));
}

bool FwFontDescription::apply(FwMLNode* node)
{
    QString family;
    int pixcelSize = 12;

    if(node->type() == FwMLNode::T_String)
    {
        family = node->cast<FwMLString>()->value();
    }
    else
    {
        FwMLObject* fontObject = node->cast<FwMLObject>();
        if(fontObject)
        {
            FwMLString* familyNode = fontObject->attribute("family")->cast<FwMLString>();
            if(familyNode)
            {
                family = QString::fromUtf8(familyNode->value());
            }

            FwMLNode* pixcelSizeNode = fontObject->attribute("pixelSize");
            if(pixcelSizeNode)
            {
                bool bOk = false;
                pixcelSize = pixcelSizeNode->toInt(&bOk);
                if(!bOk)
                {
                    pixcelSize = 0;
                }
            }
        }
    }

    if(!family.isEmpty() && pixcelSize)
    {
        m_family = family;
        m_pixcelSize = pixcelSize;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

FwFontData::FwFontData()
{
}

FwFontData::~FwFontData()
{
    release();
}

void FwFontData::release()
{
}

////////////////////////////////////////////////////////////////////////////////

FwFont::FwFont() :
    BaseClass()
{
}

FwFont::FwFont(FwFontData* data) :
    BaseClass(data)
{
}

FwFont::~FwFont()
{
}

void FwFont::release()
{
    FwFontData* data = this->data();
    if(data)
    {
        data->release();
    }
}

bool FwFont::isNull() const
{
    return data() == 0;
}

bool FwFont::operator==(const FwFont &other) const
{
    return data() == other.data();
}

bool FwFont::operator!=(const FwFont &other) const
{
    return data() != other.data();
}

QSize FwFont::stringSize(const QString& string) const
{
    FwFontData* data = this->data();
    if(data)
    {
        return data->stringSize(string);
    }
    return QSize(0, 0);
}

int FwFont::ascender() const
{
    FwFontData* data = this->data();
    if(data)
    {
        return data->ascender();
    }
    return 0;
}

int FwFont::descender() const
{
    FwFontData* data = this->data();
    if(data)
    {
        return data->descender();
    }
    return 0;
}

int FwFont::height() const
{
    FwFontData* data = this->data();
    if(data)
    {
        return data->height();
    }
    return 0;
}
