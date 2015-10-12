#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "object.h"

class Obstacle : public Object
{
    public:
        Obstacle();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // OBSTACLE_H

