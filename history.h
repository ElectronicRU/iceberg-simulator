#ifndef HISTORY_H
#define HISTORY_H
#include "singularstate.h"
#include <QList>

namespace Simulation {
class History
{
public:
    History();
    History(VelocityMap *velmap);
    ~History();
    bool load_stream(QFile *input);
    void save_stream(QFile *output);
    bool seek(qreal time);
    qreal tell();
    void add_particle(QPointF point);
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
    static const quint32 MAGIC;
    QList<SingularState> states;
    qreal maxtime;
    qreal currtime;
    VelocityMap *velocity_map;
    void chain_update();
    QPolygonF get_trace_from(int number, qreal start);
};
}

#endif // HISTORY_H
