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

unsigned short Window::window_w = 1800;
unsigned short Window::window_h = 1000;
unsigned short Window::ZombieCount = 10;
unsigned short Window::killedCount = 0;
Player* Window::player = new Player(Window::window_w/2, Window::window_h/2);
QList<QPointF> Window::coverSpots = QList<QPointF>();

Window::Window()
{
    actions.insert( Qt::Key_W, Upward );
    actions.insert( Qt::Key_A, Leftward );
    actions.insert( Qt::Key_D, Rightward );
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
    this->setMouseTracking(true);
    this->resize(window_w, window_h);
    this->show();

    scene.setSceneRect(0, 0, window_w, window_h);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(&scene);
    scene.addItem(Window::player);

    scene.addItem(new Obstacle(350, 300, 100));
    scene.addItem(new Obstacle(300, 800, 130));
    scene.addItem(new Obstacle(800, 600, 100));
    scene.addItem(new Obstacle(1000, 250, 110));
    scene.addItem(new Obstacle(1500, 400, 150));
    scene.addItem(new Obstacle(1350, 800, 100));

    for (int i = 0; i < ZombieCount; ++i)
    {
        Mouse *mouse = new Mouse;
        mouse->setPos(Window::window_w/2.0+(::sin((i * 6.28) / ZombieCount) * 200), Window::window_h/2.0+(::cos((i * 6.28) / ZombieCount) * 200));
        scene.addItem(mouse);
    }

    qApp->installEventFilter(this);
    this->timer.start(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(MainClockTick()));
}

void Window::MainClockTick()
{
    QList<QGraphicsItem *> items = scene.items();
    foreach(QGraphicsItem *item, items)
    {
        Object *object = static_cast<Object *> (item);
        object->control();
        object->physics();
        object->step();
    }
    this->calculateCoverSpots();
    turnsSinceLastAdd = (turnsSinceLastAdd+1)%20;
    if(!turnsSinceLastAdd && scene.items().contains(player))
    {
        Mouse* mouse = new Mouse();
        unsigned wall = rand()%4;
        if(wall == 0){ mouse->setPos((double)rand()/RAND_MAX*Window::window_w, 0); }
        else if(wall == 1){ mouse->setPos((double)rand()/RAND_MAX*Window::window_w, Window::window_h); }
        else if(wall == 2){ mouse->setPos(0, (double)rand()/RAND_MAX*Window::window_h); }
        else { mouse->setPos(Window::window_w, (double)rand()/RAND_MAX*Window::window_h); }
        scene.addItem(mouse);
    }
    this->update();
    for(int i = 0; i < items.size(); ++i)
    {
        if(((Object*)items[i])->destroy)
        {
            delete items[i];
            items.erase(items.begin()+(i--));
        }
    }
}

void Window::keyPressEvent(QKeyEvent* event)
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


void Window::keyReleaseEvent(QKeyEvent* event)
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

void Window::mouseMoveEvent(QMouseEvent* event)
{
  event->accept();
  Window::mouseX = event->x();
  Window::mouseY = event->y();
}

void Window::mousePressEvent(QMouseEvent* event)
{
  if(event->button() == Qt::LeftButton)
  {
    Pressed_shoot = true;
    event->accept();
  }
  else{ event->ignore(); }
}

void Window::mouseReleaseEvent(QMouseEvent* event)
{
  if(event->button() == Qt::LeftButton)
  {
    Pressed_shoot = false;
    event->accept();
  }
  else{ event->ignore(); }
}

void Window::calculateCoverSpots()
{
  coverSpots.clear();
  QList<QGraphicsItem*> items = scene.items();
  foreach(QGraphicsItem* item, items)
  {
      Object* object = static_cast<Object*>(item);
      if(object->isObstacle())
      {
        double radius = (object->boundingRect().width()+object->boundingRect().height())/4.0;
        QPointF d(object->pos());
        d -= player->pos();
        double length = sqrt(d.x()*d.x()+d.y()*d.y());
        d /= length;
        d *= length+radius+90;
        d += player->pos();
        coverSpots.append(d);
      }
  }
}
