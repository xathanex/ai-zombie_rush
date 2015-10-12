#include <QtWidgets>

#include <math.h>

#include "mouse.h"
#include "window.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    Window view;
    return app.exec();
}
