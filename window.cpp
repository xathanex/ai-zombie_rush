#include "window.h"
#include "mouse.h"
#include "object.h"
#include "player.h"
#include "projectile.h"

#include <QtOpenGL>

bool Window::Pressed_upward = FALSE;
bool Window::Pressed_leftward = FALSE;
bool Window::Pressed_rightward = FALSE;
bool Window::Pressed_downward = FALSE;
bool Window::Pressed_shoot = FALSE;

Window::Window(QObject *parent)
{
    const unsigned short window_w = 1152, window_h = 720;
    const unsigned short ZombieCount = 5;

    actions.insert( Qt::Key_W, Upward );
    actions.insert( Qt::Key_A, Leftward );
    actions.insert( Qt::Key_D, Rightward );
    actions.insert( Qt::Key_E, Shoot );
    actions.insert( Qt::Key_S, Downward );

    this->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    this->setRenderHint(QPainter::Antialiasing);
    this->setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setDragMode(QGraphicsView::NoDrag);
    this->setCursor(Qt::CrossCursor);
    this->setWindowTitle("Zombie Rush");
    this->resize(window_w, window_h);
    this->show();

    scene.setSceneRect(-window_w/2, -window_h/2, window_w, window_h);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(&scene);
    this->player = new Player();
    scene.addItem(this->player);

    for (int i = 0; i < ZombieCount; ++i)
    {
        Mouse *mouse = new Mouse;
        mouse->setPos(::sin((i * 6.28) / ZombieCount) * 200,
                      ::cos((i * 6.28) / ZombieCount) * 200);
        scene.addItem(mouse);
    }
    this->timer.start(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(MainClockTick()));
}

void Window::MainClockTick()
{
    QList<QGraphicsItem *> items = scene.items();
    this->centerOn(this->player);
    foreach(QGraphicsItem *item, items)
    {
        Object *object = static_cast<Object *> (item);
        object->control();
        object->physics();
        object->step();
    }
    scene.update();
    for(int i = 0; i < items.size(); ++i)
    {
        if(((Object*)items[i])->destroy)
        {
            delete items[i];
            items.erase(items.begin()+(i--));
        }
    }
}

void Window::keyPressEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {
        event->ignore();
        return;
    }
    Action action = actions[event->key()];
    switch ( action )
    {
    case Upward:
        Pressed_upward = TRUE;
        break;
    case Downward:
        Pressed_downward = TRUE;
        break;
    case Leftward:
        Pressed_leftward = TRUE;
        break;
    case Rightward:
        Pressed_rightward = TRUE;
        break;
    case Shoot:
        Pressed_shoot = TRUE;
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}


void Window::keyReleaseEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {
        event->ignore();
        return;
    }
    Action action = actions[event->key()];
    switch ( action )
    {
    case Upward:
        Pressed_upward = FALSE;
        break;
    case Downward:
        Pressed_downward = FALSE;
        break;
    case Leftward:
        Pressed_leftward = FALSE;
        break;
    case Rightward:
        Pressed_rightward = FALSE;
        break;
    case Shoot:
        Pressed_shoot = FALSE;
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}
