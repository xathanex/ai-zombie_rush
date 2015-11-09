#include <QDebug>

#include "player.h"
#include "window.h"
#include "projectile.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

Player::Player(double x, double y): Object() { this->setPos(x, y); }

void Player::control()
{
    QList<QGraphicsItem *> lvList = scene()->collidingItems(this);
    speed_x = 0; speed_y = 0;

    if(!lvList.isEmpty()){
        //TODO: obsluga kolizji
    }

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
    //bounding sphere
    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::blue : Qt::red);
    painter->drawEllipse(QPoint(0, 0), 20, 20);

    QImage myImage;
    myImage.load(":/images/nyanya.png");

    painter->drawImage(-17,-16,myImage);

    painter->drawLine(0,0,0,-500);

    //painter->setBrush(Qt::black);
    //painter->drawPath(shape());

}
