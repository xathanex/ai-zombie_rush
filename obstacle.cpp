#include "obstacle.h"

#include <QPainter>

    Obstacle::Obstacle()
    {}

    void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
    {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(QPoint(0, 0), 60, 60);
    }
