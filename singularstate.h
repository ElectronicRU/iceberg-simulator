#ifndef SINGULARSTATE_H
#define SINGULARSTATE_H
#include <QVector>
#include <QPoint>
#include <QPolygon>
#include "core.h"
#include "velocitymap.h"

namespace Simulation {
class SingularState
{
friend class History;
public:
    SingularState(VelocityMap *velmap);
    void load_stream(QDataStream &input);
    void save_stream(QDataStream &output);
    void add_particles(QList<QPointF> points);
    void remove_particles(QList<int> positions);
    QList<int> find_enclosed(QPolygonF shape);
    void generate_enclosed(QPolygonF shape, qreal density);
    void update_from(const SingularState *ancestor);
    void full_update(const SingularState *ancestor);
    SingularState successor();
private:
    VelocityMap *velocity_map;
    QVector<Particle> particles; // todo stress-test against a QVector
    void recalculate(int from, int to);
};
}

#endif // SINGULARSTATE_H
