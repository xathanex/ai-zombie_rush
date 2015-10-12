#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include "projectile.h"

#include <math.h>

Projectile::Projectile(double angle, double x, double y): angle(angle), x(x), y(y), life(150){}

Projectile::~Projectile(){}

void Projectile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QColor(0,0,255));
    painter->drawEllipse(x, y, 10, 10);
}

void Projectile::physics()
{
    //this->speed = ::cos(angle)*3;
    //this->slide = ::sin(angle)*3;
    if(this->life-- < 0){ this->destroy = true; }
}

QRectF Projectile::boundingRect() const
{
    return QRectF(-5,-5,10,10);
}

bool Projectile::isProjectile(){ return true; }
