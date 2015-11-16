#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "object.h"

class Projectile : public Object
{
public:
    Projectile(double = 0.0, double = 0.0, double = 0.0);
    ~Projectile();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void physics();
    QRectF boundingRect() const;

    bool isProjectile();

protected:
    double angle;
    double x, y;
    int life;
};

#endif // PROJECTILE_H
