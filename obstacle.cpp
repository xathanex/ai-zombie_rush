#include "obstacle.h"

#include <QPainter>

Obstacle::Obstacle(double x, double y, double radius): radius(radius) { this->setPos(x, y); }

QRectF Obstacle::boundingRect() const
{
    return QRectF(-radius,-radius,radius*2,radius*2);
}

QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    path.addEllipse(QPoint(0, 0), radius, radius);
    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::blue);
    painter->drawEllipse(QPointF(0, 0), this->radius, this->radius);
}

bool Obstacle::isObstacle(){ return true; }

void Obstacle::control(){}
void Obstacle::physics(){}
void Obstacle::step(){}
