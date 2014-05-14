#ifndef CORE_H
#define CORE_H
#include <QPointF>
#include <QString>
/*
 * Basic classes for the project.
 */

namespace Simulation {
const qreal STEP = 0.01;
const qreal QUANTUM = STEP * 0.1;
const qreal AREA = 1;
const qreal DENSITY = 1000;
const qreal VISCOSITY = 0.001792;
const qreal PI = 3.14;

const QString Load_Vel(":/Load_Vel.txt");
const QString Save_Vel("./Save_Vel.txt");

struct Particle
{
    QPointF position;
    QPointF velocity;
    static Particle interpolate(const Particle &p1, const Particle &p2, qreal k) {
        Particle p;
        p.position = (1 - k) * p1.position + k * p2.position;
        p.velocity = (1 - k) * p1.velocity + k * p2.velocity;
        return p;
    }
};
}

#endif // CORE_H
