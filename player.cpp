#include <QDebug>
#include <typeinfo>

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
        calculateRailLength();
           scene()->addItem(new Projectile(this->rotation(), this->x(), this->y()));
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

    painter->drawLine(0,0,0,-2000);

    //painter->setBrush(Qt::black);
    //painter->drawPath(shape());

}

void Player::step()
{


    this->setPos(this->pos().rx()+speed_x, this->pos().ry()+speed_y);
    /*
    QList<QGraphicsItem *> lvList = scene()->collidingItems(this);

    for(int i = 0; i<lvList.size();i++)
    {
        float objectX = lvList.at(i)->pos().rx();
        float objectY = lvList.at(i)->pos().ry();
        float mineX = this->pos().rx();
        float mineY = this->pos().ry();

        float lineX = objectX - mineX;
        float lineY = objectY - mineY;

        float normX = lineX;
        float normY = lineY;

        if(normX < 0)
            normX * -1;

        if(normY < 0)
            normY * -1;
        while(this->collidesWithItem(lvList.at(i)))
        {
            if(normX < normY)
            {
                if(lineX > 0)
                    this->setPos(this->pos().rx()-0.1,this->pos().ry());
                else
                    this->setPos(this->pos().rx()+0.1,this->pos().ry());
            } else {
                if(lineY > 0)
                    this->setPos(this->pos().ry()-0.1,this->pos().ry());
                else
                    this->setPos(this->pos().ry()+0.1,this->pos().ry());
            }

        }

    }
    */
    //qDebug() << QString::number(rotvel) + " " + QString::number(slide) + " " + QString::number(speed) ;
}

float Player::calculateRailLength()
{
    QList<QGraphicsItem *> items = this->scene()->items();

    float mouseX = Window::mouseX;
    float mouseY = Window::mouseY;

    float myX = this->pos().rx();
    float myY = this->pos().ry();



    /*y = ax + b

    ax + b -y = 0

    a = (-b + myY)/myX;

    -a * MouseX + mouseY = b

    -((-b + myY)/myX) * MouseX + mouseY = b

   (-myY*mouseX )/myX + mouseY = b -(b*mouseX)/myX

    (-myY*mouseX )/myX + mouseY = (b * myX)/myX - (b*mouseX)/myX

    ( (-myY*mouseX )/myX + mouseY)  /myX = b * (myX - mouseX)

    (( (-myY*mouseX )/myX + mouseY)  /myX)/(myX - mouseX)   = b

    */

    float b = ((mouseY - myX)-(myY * mouseX))/(myX - mouseX);

    float a = (myY - b)/myX;

    qDebug() << b << "  " << a;

    foreach(QGraphicsItem *item, items)
    {
        Object *object = static_cast<Object *> (item);
        if(object->isObstacle())
        {
            //qDebug() << "Obstacle!";
            float ray = 60; //promien kazdego obstacle'a




        }
    }


}


