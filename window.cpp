#include "window.h"
#include "mouse.h"
#include "object.h"
#include "player.h"
#include "projectile.h"
#include "obstacle.h"

#include <QtOpenGL>

bool Window::Pressed_upward = false;
bool Window::Pressed_leftward = false;
bool Window::Pressed_rightward = false;
bool Window::Pressed_downward = false;
bool Window::Pressed_shoot = false;
bool Window::Pressed_rotateLeft = false;
bool Window::Pressed_rotateRight = false;
int Window::mouseX;
int Window::mouseY;
Player* Window::player = new Player();

Window::Window(QObject *parent)
{
    const unsigned short window_w = 1152, window_h = 720;
    const unsigned short ZombieCount = 1;

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

    scene.setSceneRect(0, 0, window_w, window_h);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(&scene);
    //this->player = new Player();
    scene.addItem(Window::player);

    /*Obstacle *obstacle = new Obstacle;
    obstacle->setPos(400,370);
    scene.addItem(obstacle);
    obstacle = new Obstacle;
    obstacle->setPos(100,420);
    scene.addItem(obstacle);
    obstacle = new Obstacle;
    obstacle->setPos(800,600);
    scene.addItem(obstacle);*/

    for (int i = 0; i < ZombieCount; ++i)
    {
        Mouse *mouse = new Mouse;
        mouse->setPos((::sin((i * 6.28) / ZombieCount) * 200 )+300,
                      (::cos((i * 6.28) / ZombieCount) * 200)+300);
        scene.addItem(mouse);
    }

    qApp->installEventFilter(this);
    this->timer.start(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(MainClockTick()));
}

void Window::MainClockTick()
{
    QList<QGraphicsItem *> items = scene.items();
    //this->centerOn(this->player);
    foreach(QGraphicsItem *item, items)
    {
        Object *object = static_cast<Object *> (item);
        object->control();
        object->physics();
        object->step();
    }
    this->ProcessPlayer();
    this->update();
    //scene.update();
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
        Pressed_upward = true;
        break;
    case Downward:
        Pressed_downward = true;
        break;
    case Leftward:
        Pressed_leftward = true;
        break;
    case Rightward:
        Pressed_rightward = true;
        break;
    case Shoot:
        Pressed_shoot = true;
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
        Pressed_upward = false;
        break;
    case Downward:
        Pressed_downward = false;
        break;
    case Leftward:
        Pressed_leftward = false;
        break;
    case Rightward:
        Pressed_rightward = false;
        break;
    case Shoot:
        Pressed_shoot = false;
        break;
    default:
        event->ignore();
        return;
    }
    event->accept();
}

void Window::mouseMoveEvent(QMouseEvent *event){

}

bool Window::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    Window::mouseX = mouseEvent->x();
    Window::mouseY = mouseEvent->y();
  }
  return false;
}

void Window::ProcessPlayer()
{

    float linex = (Window::mouseX - this->player->x());
    float liney = (Window::mouseY - this->player->y());

    float arc = atan2(linex,-liney);

    arc = arc * 180.0 / 3.14156872;

    player->setRotation(arc);

}

