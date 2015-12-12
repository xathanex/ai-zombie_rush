#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "object.h"

class Obstacle : public Object
{
    public:
        Obstacle(double, double, double = 60);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;
        QPainterPath shape() const;
        bool isObstacle();

        void control();
        void physics();
        void step();

    private:
        double radius;
};

#endif // OBSTACLE_H

