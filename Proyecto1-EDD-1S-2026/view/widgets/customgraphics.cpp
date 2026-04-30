#include "customgraphics.h"

CustomGraphics::CustomGraphics(QWidget *parent): QGraphicsView(parent) {
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void CustomGraphics::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;

    if(event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}
