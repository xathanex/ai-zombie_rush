/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mouse.h"
#include "player.h"
#include "window.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <cmath>

static const double Pi = 3.14159265358979323846264338327950288419717;

Mouse::Mouse(): Object(), mouseEyeDirection(0), color(qrand() % 256, qrand() % 256, qrand() % 256)
{
    setRotation(qrand() % (360 * 16));
    double rot = this->rotation()*Pi/180.0;
    target_x = 100*sin(rot);
    target_y = 100*cos(rot);
}

QRectF Mouse::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}

//geometria do detekcji kolizji
QPainterPath Mouse::shape() const
{
    QPainterPath path;
    path.addRect(-10, -20, 20, 40);
    return path;
}

void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //nearby sphere

    painter->setBrush(Qt::transparent);
    painter->drawEllipse(QPoint(0, 0), 160, 160);

    //cover spots
    painter->setBrush(Qt::green);
    foreach(QPointF spot, Window::coverSpots){ painter->drawEllipse(mapFromScene(spot), 10, 10); }

    //bounding sphere
    painter->setBrush(Qt::red);
    painter->drawEllipse(QPoint(0, 0), 20, 20);

    //future pos
    painter->setBrush(Qt::white);
    QPointF futurePos(this->pos().rx()+20*speed_x, this->pos().ry()+20*speed_y);
    painter->drawEllipse(mapFromScene(futurePos), 20, 20);

    //collisions
    painter->setBrush(Qt::black);
    foreach(QPointF point, this->collisions)
    {
        painter->drawEllipse(mapFromScene(point), 20, 20);
    }

    //body
    painter->setBrush(color);
    painter->drawEllipse(-10, -20, 20, 40);

    painter->setBrush(Qt::white);
    painter->drawEllipse(-10, -17, 8, 8);
    painter->drawEllipse(2, -17, 8, 8);

    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    painter->drawEllipse(QRectF(-8.0 + mouseEyeDirection, -17, 4, 4));
    painter->drawEllipse(QRectF(4.0 + mouseEyeDirection, -17, 4, 4));

    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);

}

void Mouse::step()
{
    Object::step();
    foreach(QGraphicsItem* it, scene()->collidingItems(this))
    {
        Object* item = (Object*)it;
        if((item->isProjectile() && !(item->destroy))){ this->destroy = true; break; }
    }
    if(this->collidingItems().indexOf((QGraphicsItem*)Window::player) != -1)
    {
      if(Player::damage <= 10)
        Player::damage += 1;
    }
}

void Mouse::control()
{
    attack_player = isInGroup();
    if(attack_player)
    {
        target_x = Window::player->x() - this->x();
        target_y = Window::player->y() - this->y();
    }
    else { this->hide(); }
    this->wander();

    this->seek(target_x, target_y);
    this->avoidObstacles();
}

bool Mouse::isInGroup()
{
    unsigned mousesNearbyCount = 0;
    foreach(QGraphicsItem* item, this->scene()->items())
    {
        Object* object = (Object*) item;
        if(!object->isProjectile() && !object->isObstacle() && object != Window::player && object != this)
        {
            QPointF d(object->pos());
            d -= this->pos();
            double dist = sqrt(d.x()*d.x()+d.y()*d.y());
            if(dist <= 160)
            {
                ++mousesNearbyCount;
                if(mousesNearbyCount > 3){ return true; }
            }
        }
    }
    return false;
}

void Mouse::hide()
{
    QPointF p(this->pos());
    double dist = 999999;
    foreach(QPointF coverSpot, Window::coverSpots)
    {
        QPointF t(coverSpot);
        t -= this->pos();
        double d = t.manhattanLength();
        if(d < dist || (double)rand()/RAND_MAX < 0.2)
        {
            dist = d;
            p = coverSpot;
        }
    }
    p -= this->pos();
    target_x += p.x();
    target_y += p.y();
}

void Mouse::wander()
{
    const double jitter_radius = 50;
    const double target_distance = 200;
    const QRectF field(0, 0, Window::window_w, Window::window_h);

    do
    {
        double jitter_angle = (double)rand()/RAND_MAX*2*Pi;
        target_x += jitter_radius*sin(jitter_angle);
        target_y += jitter_radius*cos(jitter_angle);
        double length = sqrt(target_x*target_x+target_y*target_y);
        target_x = target_x/length*target_distance;
        target_y = target_y/length*target_distance;
    }
    while(!field.contains(0.4*target_x+this->pos().rx(), 0.4*target_y+this->pos().ry()));
}

void Mouse::seek(double dx, double dy)
{
    const double current_weight = 0.7;
    const double desired_weight = 0.3;
    const double max_rot_speed = 30;

    speed_x = speed_x*current_weight+dx*desired_weight;
    speed_y = speed_y*current_weight+dy*desired_weight;

    double angle = atan2(dx,-dy);
    angle = angle*180.0/Pi;
    double dAlpha = angle-this->rotation();
    while(dAlpha > 180){ dAlpha -= 360; }
    while(dAlpha < -180){ dAlpha += 360; }
    angle = this->rotation()+std::max(std::min(dAlpha, max_rot_speed), -max_rot_speed);
    this->setRotation(angle);
    limit_speed();
}

void Mouse::avoidObstacles()
{
    const double mult = 1.5;
    QPointF futurePos(this->pos());
    futurePos.rx() += 10*speed_x;
    futurePos.ry() += 10*speed_y;
    double d_speed_x = 0;
    double d_speed_y = 0;

    if(futurePos.rx() < 20){ d_speed_x = mult*futurePos.rx(); }
    else if(futurePos.rx() > Window::window_w){ d_speed_x = mult*(Window::window_w-futurePos.rx()); }

    if(futurePos.ry() < 20){ d_speed_y = mult*futurePos.ry(); }
    else if(futurePos.ry() > Window::window_h-20){ d_speed_y = mult*(Window::window_h-20-futurePos.ry()); }

    double speed = sqrt(speed_x*speed_x+speed_y*speed_y);
    double speed_wersor_x = speed_x/speed;
    double speed_wersor_y = speed_y/speed;

    this->collisions = QList<QPointF>();
    foreach(QGraphicsItem* it, this->scene()->items())
    {
        Object* item = (Object*)it;
        if(item->isObstacle())
        {
            double radius = (item->boundingRect().height()+item->boundingRect().width())/4.0;
            radius += (this->boundingRect().height()+this->boundingRect().width())/4.0;

            double v_x = item->pos().rx()-this->pos().rx();
            double v_y = item->pos().ry()-this->pos().ry();

            double vs = speed_wersor_x*v_x+speed_wersor_y*v_y;
            if(vs > 0)
            {
                double vs_x = vs*speed_wersor_x;
                double vs_y = vs*speed_wersor_y;

                double dx = item->pos().rx()-(this->pos().rx()+vs_x);
                double dy = item->pos().ry()-(this->pos().ry()+vs_y);

                double d = sqrt(dx*dx+dy*dy);
                if(d < radius)
                {
                    double dd = sqrt(radius*radius-d*d);
                    vs -= dd;
                    vs_x = vs*speed_wersor_x;
                    vs_y = vs*speed_wersor_y;
                }

                if(d <= radius)
                {
                    //vs_x & vs_y -- pozycja względna punktu kolizji
                    double dist = sqrt(vs_x*vs_x+vs_y*vs_y);
                    if(dist <= 100)
                    {
                        this->collisions.append(QPointF(vs_x+this->pos().rx(), vs_y+this->pos().ry()));
                        //hamowanie
                        d_speed_x -= speed_wersor_x*MAX_SPEED*10/dist;
                        d_speed_y -= speed_wersor_y*MAX_SPEED*10/dist;
                        //odbicie
                        double dsx = vs_x+this->pos().rx()-item->pos().rx();
                        double dsy = vs_y+this->pos().ry()-item->pos().ry();
                        double od = sqrt(dsx*dsx+dsy*dsy);
                        d_speed_x -= -speed_wersor_y*100/od*MAX_SPEED;
                        d_speed_y -= speed_wersor_x*100/od*MAX_SPEED;
                    }
                }
            }
        }
    }

    double sx = speed_x + d_speed_x;
    double sy = speed_y + d_speed_y;

    seek(sx, sy);
}
