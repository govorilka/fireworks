#include <QtCore/qdebug.h>

#include "fwtextprimitive.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwpen.h"

#include "fwcore/fwml.h"

#include "fwtypography/fwjustification.h"

FwTextPrimitive::FwTextPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_fixedHeight(false)
{
    setPen(new FwPen(1, FwColor(0xFFFFFFFF)));
}

FwTextPrimitive::~FwTextPrimitive()
{
}

void FwTextPrimitive::setText(const QString& text)
{
    if(m_text != text)
    {
        m_text = text;
        updateText();
    }
}

void FwTextPrimitive::setFont(const FwFont& font)
{
    if(m_font != font)
    {
       m_font = font;
       updateText();
    }
}

void FwTextPrimitive::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    BaseClass::geometryChangedEvent(oldRect, rect);
    updateTextLayout(rect);
}

void FwTextPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    BaseClass::paint(painter, clipRect);

    FwPen* pen = this->pen();
    if(!m_strings.isEmpty() && pen)
    {
        painter->setColor(pen->color());
        painter->setFont(m_font);

        FwTextString* string = m_strings.data();
        for(int i = 0; i < m_strings.count(); i++, string++)
        {
            painter->drawString(string->x(), string->y(), string->utf8());
        }
    }
}

void FwTextPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwFont font = createFont(object, "font");
    if(!font.isNull())
    {
        setFont(font);
    }

    FwMLString* textNode = object->attribute("text")->cast<FwMLString>();
    if(textNode)
    {
        setText(QString::fromUtf8(textNode->value()));
    }

    FwMLNode* fixedHeightNode = object->attribute("fixedHeight");
    if(fixedHeightNode)
    {
        bool bOk = false;
        bool fixedHeight = fixedHeightNode->toBool(&bOk);
        if(bOk)
        {
            setFixedHeight(fixedHeight);
        }
    }

    BaseClass::apply(object);

    update();
}

void FwTextPrimitive::updateTextLayout(QRect& rect)
{
    if(m_text.isEmpty())
    {
        m_strings = QVector<FwTextString>();
    }
    else
    {
        FwJustification jEngine;
        jEngine.setBlockRect(rect);
        jEngine.setFont(m_font);
        m_strings = jEngine.processing(m_text, m_fixedHeight);
        if(!m_fixedHeight)
        {
            rect.setHeight(jEngine.blockRect().height());
        }
    }
}

void FwTextPrimitive::setFixedHeight(bool enable)
{
    if(m_fixedHeight != enable)
    {
        prepareGeometryChanged();
        m_fixedHeight = enable;
        updateText();
        update();
    }
}
