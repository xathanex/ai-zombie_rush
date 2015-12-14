#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "object.h"

class Projectile : public Object
{
public:
    Projectile(double = 0.0, double = 0.0, double = 0.0);
    ~Projectile();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void control();
    void physics();
    void step();

    QRectF boundingRect() const;

    bool isProjectile();
    bool active;
    float length;

protected:
    double angle;
    double x, y;
};

#endif // PROJECTILE_H
