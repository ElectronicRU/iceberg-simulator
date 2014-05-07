#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
        /*VelocityMap *map = new VelocityMap(19,2,true);
    */

    return a.exec();
}
