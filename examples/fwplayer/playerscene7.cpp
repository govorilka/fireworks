#include "playerscene7.h"


#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwguievent.h"

#include "fwgui/widgets/fwpixmapbar.h"


PlayerScene7::PlayerScene7(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_bar(new FwPixmapBar("bar", this))
{
    m_bar->show();
}


void PlayerScene7::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        view()->setActiveScene(1);
        event->accept();
        return;
    }
}
