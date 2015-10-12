#include <QDebug>

#include "player.h"
#include "window.h"
#include "projectile.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

Player::Player(): Object() {}

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
       scene()->addItem(new Projectile(this->rotation()*Pi/180, this->x(), this->y()));
    }
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //bounding sphere
    painter->setBrush(Qt::green);
    painter->drawEllipse(QPoint(0, 0), 20, 20);

    //body
    painter->setBrush(Qt::black);
    painter->drawEllipse(-14, -20, 28, 40);

    painter->setBrush(Qt::white);
    painter->drawEllipse(-10, -17, 8, 8);
    painter->drawEllipse(2, -17, 8, 8);

    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(-8.0 , -17, 4, 4));
    painter->drawEllipse(QRectF(4.0 , -17, 4, 4));

    painter->setBrush(Qt::gray);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}
