#include <QDebug>
#include <typeinfo>

#include "player.h"
#include "window.h"
#include "projectile.h"


static const double Pi = 3.14159265358979323846264338327950288419717;

Player::Player(double x, double y): Object() { this->setPos(x, y); this->cooldown = 0; }

void Player::control()
{
    speed_x = 0; speed_y = 0;

    if(Window::Pressed_upward)
    {
        speed_y -= MAX_SPEED;
    }
    if(Window::Pressed_downward)
    {
        speed_y += MAX_SPEED;
    }
    if(Window::Pressed_leftward)
    {
            speed_x -= MAX_SPEED;
    }
    if(Window::Pressed_rightward)
    {
            speed_x += MAX_SPEED;
    }
    if(Window::Pressed_shoot)
    {
        if(cooldown <= 0)
        {
           scene()->addItem(new Projectile(this->rotation(), this->x(), this->y()));
           this->cooldown = 200;
        }
    }
}

QRectF Player::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addEllipse(QPoint(0, 0), 20, 20);
    return path;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(scene()->collidingItems(this).isEmpty())
    {
        if(this->cooldown > 0)
        {
            painter->setBrush(Qt::yellow);
        } else {
            painter->setBrush(Qt::blue);
        }

    } else {
        painter->setBrush(Qt::red);
    }

    painter->drawEllipse(QPoint(0, 0), 20, 20);

    QImage myImage;
    myImage.load(":/images/nyanya.png");

    painter->drawImage(-17,-16,myImage);

    painter->drawLine(0,0,0,-2000);

}


void Player::step()
{


    this->setPos(this->pos().rx()+speed_x, this->pos().ry()+speed_y);
    this->cooldown--;
}

void Player::physics()
{
    this->cooldown--;
    //qDebug() << "Cooldown:" << this->cooldown;
}



