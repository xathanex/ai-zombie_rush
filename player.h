#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui>

#include "object.h"

class Player : public Object
{
public:
    Player();
    void control();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // PLAYER_H
