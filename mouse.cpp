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
#include "window.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>

#include <cmath>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Mouse::Mouse(): Object(), mouseEyeDirection(0), color(qrand() % 256, qrand() % 256, qrand() % 256)
{
    setRotation(qrand() % (360 * 16));
    double rot = this->rotation()*Pi/180.0;
    wander_target_x = 100*sin(rot);
    wander_target_y = 100*cos(rot);
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
    //bounding sphere
    painter->setBrush(Qt::red);
    painter->drawEllipse(QPoint(0, 0), 20, 20);

    //wander_targer
    painter->setBrush(Qt::white);
    painter->drawEllipse(mapFromScene(QPoint(this->pos().rx()+wander_target_x, this->pos().ry()+wander_target_y)), 20, 20);

    //body?
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
    foreach(QGraphicsItem* item, scene()->collidingItems(this))
    {
        if(((Object*)item)->isProjectile()){ this->destroy = true; }
    }
}

void Mouse::control()
{
    this->wander();
    this->seek(wander_target_x, wander_target_y);
}

void Mouse::wander()
{
    const double jitter_radius = 50;
    const double target_distance = 200;

    double jitter_angle = (double)rand()/RAND_MAX*2*Pi;

    wander_target_x += jitter_radius*sin(jitter_angle);
    wander_target_y += jitter_radius*cos(jitter_angle);
    double length = sqrt(wander_target_x*wander_target_x+wander_target_y*wander_target_y);
    wander_target_x = wander_target_x/length*target_distance;
    wander_target_y = wander_target_y/length*target_distance;
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
}
