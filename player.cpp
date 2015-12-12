#include <QDebug>
#include <typeinfo>

#include "player.h"
#include "window.h"
#include "projectile.h"


static const double Pi = 3.14159265358979323846264338327950288419717;

Player::Player(double x, double y): Object(), cooldown(0) { this->setPos(x, y);}

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
           this->cooldown = 100;
        }
    }

    float linex = (Window::mouseX - this->pos().rx());
    float liney = (Window::mouseY - this->pos().ry());
    float arc = atan2(linex,-liney);
    arc = arc * 180.0 / 3.14156872;
    this->setRotation(arc);
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
}


void Player::step()
{
    Object::step();
    if(cooldown){ --cooldown; }
}
