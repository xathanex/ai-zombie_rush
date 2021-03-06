
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include "projectile.h"
#include "window.h"
#include "player.h"
#include <math.h>

Projectile::Projectile(double angle, double x, double y): angle(angle), x(x), y(y)
{
    this->setRotation(angle-90);
    this->setPos(x,y);
    this->active = false;
    this->length = 150000;
    QList<QGraphicsItem *> lvList = Window::player->scene()->collidingItems(this);
    for(int i = 0; i<lvList.size();i++)
    {
        Object *object = static_cast<Object *> (lvList.at(i));
        if(object->isObstacle())
        {
           float ox =  object->pos().rx();
           float oy=  object->pos().ry();

           float rayLength = sqrt((ox-this->pos().rx())*(ox-this->pos().rx())+(oy-this->pos().ry())*(oy-this->pos().ry()));

           //rayLength -= (object->boundingRect().height())/2;

           if(rayLength < length)
               length = rayLength;
        }
    }
}

Projectile::~Projectile(){}

void Projectile::control(){}
void Projectile::physics(){}
void Projectile::step()
{
  if(!this->active){ this->active = true; }
  else { this->destroy = true; }
}

void Projectile::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* )
{
    QBrush *brush = new QBrush();
    brush->setStyle(Qt::SolidPattern);
    brush->setColor(QColor(0,0,255));
    painter->setPen(Qt::NoPen);  // brak borderow wokol lasera
    painter->setBrush(*brush);
    painter->drawRect(QRect(0,0,length,3));

    //
}

QRectF Projectile::boundingRect() const
{
    return QRectF(0,0,this->length,3);
}

bool Projectile::isProjectile(){ return true; }
