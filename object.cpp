#include "object.h"
#include "window.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <cmath>

Object::Object(): MAX_SPEED(3), speed_x(0), speed_y(0), destroy(false){}

QRectF Object::boundingRect() const
{
    return QRectF(-10,-10,20,20);
}

void Object::physics()
{
  if(!(this->isProjectile()) && !(this->isObstacle()))
  {
    limit_speed();
    double my_radius = (this->boundingRect().height()+this->boundingRect().width())/4.0;
    foreach(QGraphicsItem* it, this->collidingItems())
    {
        Object* item = (Object*)it;
        if(!(item->isProjectile()))
        {
          double dx = item->pos().rx() - this->pos().rx();
          double dy = item->pos().ry() - this->pos().ry();
          double dist = sqrt(dx*dx+dy*dy);
          double radius = (item->boundingRect().width()+item->boundingRect().height())/4.0;

          if(dist < radius+my_radius)
          {
              double trans_length = radius+my_radius-dist;
              this->setPos(this->pos().rx()-dx/dist*trans_length, this->pos().ry()-dy/dist*trans_length);
          }
       }
    }
  }
}

void Object::step()
{
    if(this->isProjectile() || this->isObstacle()){ return; }

    double my_radius = (this->boundingRect().height()+this->boundingRect().width())/4.0;
    double x = this->pos().rx()+speed_x;
    x = std::min(std::max(my_radius, x), Window::window_w - my_radius);
    double y = this->pos().ry()+speed_y;
    y = std::min(std::max(my_radius, y), Window::window_h - my_radius);
    this->setPos(x, y);
}

bool Object::isProjectile(){ return false; }
bool Object::isObstacle(){ return false; }


void Object::limit_speed()
{
    double l = pow(speed_x*speed_x + speed_y*speed_y, 0.5);
    if(l > MAX_SPEED)
    {
      speed_x = speed_x/l*MAX_SPEED;
      speed_y = speed_y/l*MAX_SPEED;
    }
}

