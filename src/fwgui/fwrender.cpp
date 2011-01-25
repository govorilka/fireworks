#include "fwrender.h"

/*!
\class FwRender
Интерфейс для создания новых рендеров.
*/

FwRender::FwRender() :
    m_startX(0),
    m_startY(0),
    nullPos(0, 0),
    rect(0, 0, 0, 0),
    opacity(1.0),
    colorBuffer(0x00, 0x00, 0x00, 0xFF)
{
}

FwRender::~FwRender()
{
}

void FwRender::prepareDrawSurface(FwPixmapData* surface)
{
    Q_UNUSED(surface);
}
