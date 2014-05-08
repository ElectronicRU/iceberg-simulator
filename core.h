#ifndef CORE_H
#define CORE_H
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


#endif // CORE_H
