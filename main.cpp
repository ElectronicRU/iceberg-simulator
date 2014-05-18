#include "mainwindow.h"
#include <QApplication>
#include "history.h"
#include "velocitymap.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    std::cout << "hello world" << std::endl;
    Visual::MainWindow w;
    w.show();


    QFile f(":/Load_Vel.txt");
    if (!f.open(QIODevice::ReadOnly))
        std::cout << "wtf" << std::endl;
    Simulation::History history(Simulation::VelocityMap::LoadText(&f));
    f.close();
    QList<QPointF> poss;
    poss << QPointF(0, 0) << QPointF(0, 1) <<  QPointF(7, 7);
    std::cout << history.get_velocity_map() << std::endl;
    history.add_particles(poss);
    history.setMaxtime(5);
    w.setHistory(&history);

    return a.exec();
}
