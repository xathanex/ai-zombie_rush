#include "object.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <cmath>

Object::Object(): MAX_SPEED(10), speed_x(0), speed_y(0), destroy(false){}

QRectF Object::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}


void Object::control(){}

void Object::physics()
{
  double l = pow(speed_x*speed_x + speed_y*speed_y, 0.5);
  if(l > MAX_SPEED)
  {
    speed_x = speed_x/l*MAX_SPEED;
    speed_y = speed_y/l*MAX_SPEED;
  }
}

void Object::step()
{
    //this->setRotation(this->rotation() - rotvel);
    this->setPos(mapToParent(speed_x, speed_y));
    //qDebug() << QString::number(rotvel) + " " + QString::number(slide) + " " + QString::number(speed) ;
}

void Object::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::gray);
    painter->drawRect(-10, -10, 20, 20);
}

bool Object::isProjectile(){ return false; }
