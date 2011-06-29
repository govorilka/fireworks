#include "fwtextview.h"

#include "fwgui/primitives/fwtextprimitive.h"


FwTextView::FwTextView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_text(new FwTextPrimitive("text", this))
{
    m_text->prepareGeometryChanged();
    m_text->setPosition(Fw::HP_CenterDock, Fw::VP_MiddleDock);
    m_text->link(geometry());
    m_text->update();
}

FwTextView::~FwTextView()
{
}
