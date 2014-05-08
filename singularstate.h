#ifndef SINGULARSTATE_H
#define SINGULARSTATE_H
#include <QVector>
#include <QPoint>
#include <QPolygon>
#include "core.h"

namespace Simulation {
class SingularState
{
public:
    SingularState();
    void load_stream(QDataStream &input);
    void save_stream(QDataStream &output);
    void add_particles(QList<QPointF> points);
    void remove_particles(QList<int> positions);
    QList<int> find_enclosed(QPolygonF shape);
    void generate_enclosed(QPolygonF shape, qreal density);
    void update_from(SingularState* ancestor);
    void full_update(const SingularState &ancestor);
private:
    QVector<Particle> particles; // todo stress-test against a QVector
    void recalculate(int from, int to);
};
}

#endif // SINGULARSTATE_H
