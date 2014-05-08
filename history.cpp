#include "history.h"

#include <QDataStream>

using namespace Simulation;

History::History(SingularState s)
    : states(), cursor(0), maxtime(0)
{
    states << s;
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

void History::continue_history(qreal time)
{
    maxtime = time;
    int delta = qRound(maxtime / QUANTUM) - states.size();
    SingularState *st = &states.last();
    for (int i = 0; i < delta; i++) {
        // I feel bad, but I have hopes for C++11 move semantics. Please, let no deep copy occur!
        // Seriously though, managing a list of ALLOCATED SingularStates by hand is more hellish.
        states.append(SingularState());
        states.last().full_update(st);
        st = &states.last();
    }
}

void History::seek(qreal time)
{
    time = qBound((qreal)0, time, maxtime);
    cursor = qRound(time / QUANTUM);
}

qreal History::tell()
{
    return cursor * QUANTUM;
}

void History::add_particles(QList<QPointF> points)
{
    states[cursor].add_particles(points);
    chain_update();
}

void History::remove_particles(QList<int> positions)
{
    for (int i = cursor; i < states.size(); i++)
    {
        states[i].remove_particles(positions);
    }
}

void History::chain_update()
{
    for (int i = cursor + 1; i < states.size(); i++)
    {
        states[i].full_update(&states[i-1]);
    }
}




