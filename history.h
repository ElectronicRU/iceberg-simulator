#ifndef HISTORY_H
#define HISTORY_H
#include "singularstate.h"
#include <QList>

namespace Simulation {
class History
{
public:
    History(SingularState);
    History(VelocityMap *velmap);
    void load_stream(QDataStream input);
    void save_stream(QDataStream output);
    bool seek(qreal time);
    qreal tell();
    void add_particles(QList<QPointF> points);
    void remove_particles(QList<int> positions);
    void full_update();
    QPolygonF get_trace(int number);
    QPolygonF get_future_trace(int number);
    Particle operator[](int i);
    Particle at(int i);
    int size() const { return states[0].particles.size(); }
    qreal getMaxtime() const;
    void setMaxtime(const qreal &value);

    VelocityMap *get_velocity_map();

private:
    QList<SingularState> states;
    qreal maxtime;
    qreal currtime;
    void chain_update();
    QPolygonF get_trace_from(int number, qreal start);
};
}

#endif // HISTORY_H
