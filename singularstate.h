#ifndef SINGULARSTATE_H
#define SINGULARSTATE_H
#include <QVector>
#include <QPoint>
#include <QPolygon>
#include <QDataStream>
#include "core.h"
#include "velocitymap.h"

namespace Simulation {
class SingularState
{
friend class History;
public:
    SingularState();
    void add_particles(QList<QPointF> points, const VelocityMap *velocity_map);
    void remove_particles(QList<int> positions);
    QList<int> find_enclosed(QPolygonF shape);
    void generate_enclosed(QPolygonF shape, qreal density, const VelocityMap *velocity_map);
    void update_from(const SingularState *ancestor, const VelocityMap *velocity_map);
    void full_update(const SingularState *ancestor, const VelocityMap *velocity_map);
    SingularState successor(const VelocityMap *velocity_map);

    friend QDataStream &operator>>(QDataStream&, SingularState&);
    friend QDataStream &operator<<(QDataStream&, const SingularState&);
private:
    QVector<Particle> particles;
    void recalculate(int from, int to, const VelocityMap *velocity_map);
};
/*
QDataStream &operator>>(QDataStream &input, Simulation::SingularState &self);
QDataStream &operator<<(QDataStream &output, Simulation::SingularState &self);
*/
}




#endif // SINGULARSTATE_H
