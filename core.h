#ifndef CORE_H
#define CORE_H
#include <QPointF>
#include <string>
/*
 * Basic classes for the project.
 */

namespace Simulation {
const qreal QUANTUM = 0.01;
struct Particle
{
    QPointF position;
    QPointF velocity;
    // Not really much more to it, alas.
};
}

const qreal AREA = 1;
const qreal DENSITY = 1000;

const std::string Load_Vel = "../Load_Vel.txt";
const std::string Save_Vel = "../Save_Vel.txt";

#endif // CORE_H
