#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "object.h"

class Obstacle : public Object
{
    public:
        Obstacle();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;
        QPainterPath shape() const;
        bool isObstacle();
};

#endif // OBSTACLE_H

