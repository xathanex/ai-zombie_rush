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

    const double MAX_SPEED;
};

#endif // OBJECT_H
