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

    void limit_speed();

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) = 0;

    const double MAX_SPEED;
    double speed_x, speed_y;

    bool destroy;
    virtual bool isProjectile();
    virtual bool isObstacle();

    double rotvel;          // predkosc obrotowa (Rotational Velocity)
    static const double rotfriction = 0.9750;     // obrotowy
    static const double rotacc = 0.5;           // przyspieszenie obrotowe
};

#endif // OBJECT_H
