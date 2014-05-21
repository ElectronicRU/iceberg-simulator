#include "history.h"
#include <QtCore/QtMath>
#include <QDataStream>

namespace Simulation {

const quint32 History::MAGIC = 0x1CEBE;



History::History() :
    History(nullptr)
{
}

History::History(VelocityMap *velmap) :
    states(), maxtime(0), currtime(0), velocity_map(velmap)
{
    states << SingularState();
}

History::~History()
{
    delete velocity_map;
}

bool History::load_stream(QFile *f)
{
    QDataStream input(f);
    quint32 magic;
    input >> magic;
    if (magic != MAGIC)
        return false;
    velocity_map = VelocityMap::load_stream(input);
    input >> states >> maxtime >> currtime;
    return true;
}

void History::save_stream(QFile *f)
{
    QDataStream output(f);
    output << MAGIC;
    velocity_map->save_stream(output);
    output << states << maxtime << currtime;

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

void History::add_particle(QPointF point)
{
    states[0].add_particle(point, velocity_map);
    chain_update();
}

void History::add_particles(QList<QPointF> points)
{
    for (auto point: points)
        states[0].add_particle(point, velocity_map);
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
        SingularState *s = &states.last();
        states.append(SingularState());
        states.last().full_update(s, velocity_map);
    }
    if (delta < 0)
    {
        states.erase(states.begin() + newsize, states.end());
    }
}

VelocityMap *History::get_velocity_map()
{
    return velocity_map;
}


void History::chain_update()
{
    for (int i = 1; i < states.size(); i++)
    {
        states[i].update_from(&states[i-1], velocity_map);
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

}



