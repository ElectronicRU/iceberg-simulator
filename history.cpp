#include "history.h"
#include <QtCore/QtMath>
#include <QDataStream>

using namespace Simulation;

History::History(SingularState s)
    : states(), maxtime(0), currtime(0)
{
    states << s;
}

History::History(VelocityMap *velmap) :
    History(SingularState(velmap))
{
}

void Simulation::History::load_stream(QDataStream input)
{
    qint32 icursor;
    //input >> icursor >> states >> maxtime;
    //cursor = icursor;
}

void Simulation::History::save_stream(QDataStream output)
{
    //output << (qint32) cursor << states << maxtime;
}

bool History::seek(qreal time)
{
    time = qBound((qreal)0, time, maxtime);
    if (qAbs(currtime - time) > QUANTUM)
    {
        currtime = time;
        return true;
    }
    else
    {
        return false;
    }
}

qreal History::tell()
{
    return currtime;
}

void History::add_particles(QList<QPointF> points)
{
    states[0].add_particles(points);
    chain_update();
}

void History::remove_particles(QList<int> positions)
{
    for (int i = 0; i < states.size(); i++)
    {
        states[i].remove_particles(positions);
    }
}

void History::full_update()
{
    chain_update();
}

QPolygonF History::get_trace(int number)
{
    return get_trace_from(number, 0);
}

QPolygonF History::get_future_trace(int number)
{
    return get_trace_from(number, currtime);
}

Particle History::operator[](int i)
{
    return at(i);
}

Particle History::at(int i)
{
    int cursor = qFloor(currtime / STEP);
    qreal delta = currtime / STEP - cursor;
    return Particle::interpolate(states[cursor].particles[i],
                                 states[cursor + 1].particles[i], delta);
}

qreal History::getMaxtime() const
{
    return maxtime;
}

void History::setMaxtime(const qreal &value)
{
    maxtime = value;
    int oldsize = states.size(), newsize = qFloor(maxtime / STEP) + 1 + 1;
    int delta = newsize - oldsize;
    for (int i = 0; i < delta; i++) {
        // I feel bad, but I have hopes for C++11 move semantics. Please, let no deep copy occur!
        // Seriously though, managing a list of ALLOCATED SingularStates by hand is more hellish.
        states.append(states.last().successor());
    }
    if (delta < 0)
    {
        states.erase(states.begin() + newsize, states.end());
    }
}

VelocityMap *History::get_velocity_map()
{
    return states[0].velocity_map;
}


void History::chain_update()
{
    for (int i = 1; i < states.size(); i++)
    {
        states[i].full_update(&states[i-1]);
    }
}

QPolygonF History::get_trace_from(int number, qreal start)
{
    QPolygonF result;
    int startAt = qCeil(start / STEP);
    if (startAt * STEP - start > QUANTUM)
    {
        result.append(this->at(number).position);
    }
    auto ii = states.begin() + startAt;
    while (ii != states.end())
    {
        result.append(ii->particles[number].position);
        ++ii;
    }
    return result;
}




