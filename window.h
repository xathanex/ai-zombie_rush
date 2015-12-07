#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets>

class Player;
class Object;

class Window : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Window();
    QGraphicsScene scene;
    QTimer timer;

    static Player* player;

    static bool Pressed_upward;
    static bool Pressed_leftward;
    static bool Pressed_rightward;
    static bool Pressed_downward;
    static bool Pressed_rotateLeft;
    static bool Pressed_rotateRight;
    static bool Pressed_shoot;
    enum Action { Upward, Leftward, Rightward, Downward, Shoot };
    QMap<int,Action> actions;

    static int mouseX;
    static int mouseY;

    static unsigned short window_w, window_h;
    static unsigned short ZombieCount;

    void ProcessPlayer();


    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:
    void keyPressEvent( QKeyEvent *event );
    void keyReleaseEvent( QKeyEvent *event );

private slots:
    void MainClockTick();
};

#endif // WINDOW_H
