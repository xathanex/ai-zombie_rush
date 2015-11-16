#include "obstacle.h"

#include <QPainter>

    Obstacle::Obstacle()
    {}

    QRectF Obstacle::boundingRect() const
    {
        return QRectF(-60,-60,120,120);
    }

    QPainterPath Obstacle::shape() const
    {
        QPainterPath path;
        path.addEllipse(QPoint(0, 0), 60, 60);
        return path;
    }

    void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
    {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(QPoint(0, 0), 60, 60);

        //painter->setBrush(Qt::black);
        //painter->drawPath(shape());

    }

    bool Obstacle::isObstacle(){ return true; }

