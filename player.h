#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui>

#include "object.h"

class Player : public Object
{
public:
    Player(double x, double y);
    void control();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void step();
    float calculateRailLength();
    int cooldown;
    void physics();
};

#endif // PLAYER_H
