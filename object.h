#ifndef OBJECT_H
#define OBJECT_H

#include <QGraphicsItem>

class Object : public QGraphicsItem
{
public:
    Object();
    QRectF boundingRect() const;


    virtual void control();
    virtual void physics();
    virtual void step();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    double speed_x, speed_y;

    bool destroy;
    virtual bool isProjectile();

    double rotvel;          // predkosc obrotowa (Rotational Velocity)
    static const double rotfriction = 0.9750;     // obrotowy
    static const double rotacc = 0.5;           // przyspieszenie obrotowe

    const double MAX_SPEED;
};

#endif // OBJECT_H
