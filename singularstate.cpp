#include "singularstate.h"
#include <QtCore/qmath.h>
#include <random>

using namespace Simulation;
SingularState::SingularState()
    : particles()
{
    // XXX when we implement VelocityMap, return here.
}

void SingularState::add_particles(QList<QPointF> points)
{
    for (auto p: points)
    {
        Particle new_p;
        new_p.position = p; // XXX calculate the velocity properly
        particles.append(new_p);
    }
}

void SingularState::remove_particles(QList<int> positions)
{
    std::sort(positions.begin(), positions.end());
    positions.append(particles.length());
    int ifrom = 0, ito = 0;
    for (int i: positions)
    {
        while (ifrom < i)
        {
            std::swap(particles[ifrom++], particles[ito++]);
        }
        ifrom++;
    }
    particles.remove(ito, (ifrom-1)-ito);
}

QList<int> SingularState::find_enclosed(QPolygonF shape)
{
    QList<int> res;
    int l = particles.size();
    for (int i = 0; i < l; i++) {
        if (shape.containsPoint(particles[i].position, Qt::OddEvenFill))
            res.append(i);
    }
    return res;
}

void SingularState::generate_enclosed(QPolygonF shape, qreal density)
{
    //Calculating the amount of points needed.
    qreal area = 0;
    QPointF s = shape[0];
    for (i = 2; i < shape.size(); i++) {
        QPointF v1 = shape[i-1] - s, v2 = shape[i] - s;
        area += v1.x() * v2.y() - v1.y() * v2.x();
    }
    area = qFabs(area);
    int number_of_points = qRound(area * density);
    QRectF rect = shape.boundingRect();
    QList<QPointF> points;
    qreal x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    // The following is a bit cheesy, but indubitably simple and fast enough for real-life purposes.
    std::random_device rd;
    std::default_random_engine engine(rd());
    uniform_real_distribution<qreal> randx(x1, std::nextafter(x2, std::numeric_limits<qreal>::max()));
    uniform_real_distribution<qreal> randy(y1, std::nextafter(y2, std::numeric_limits<qreal>::max());
    while (number_of_points)
    {
        QPointF p(randx(engine), randy(engine));
        if (shape.containsPoint(p, Qt::OddEvenFill))
        {
            number_of_points--;
            points.append(p);
        }
    }
    add_particles(points);
}



void SingularState::update_from(SingularState *ancestor)
{
    int old_size = particles.size(), new_size = ancestor->particles.size();
    // here, assuming new_size >= old_size
    particles.resize(new_size);
    std::copy(&ancestor->particles[old_size], &ancestor->particles[new_size], &particles[old_size]);
    recalculate(old_size, new_size);
}

void SingularState::full_update(SingularState *ancestor)
{
    particles.resize(ancestor->particles.size());
    std::copy(ancestor->particles.begin(), ancestor->particles.end(), particles.begin());
    recalculate(0, particles.size());
}

void SingularState::recalculate(int from, int to)
{
    for (int i = from; i < to; i++) {
        qreal h = QUANTUM;
        Particle &p = particles[i];
        QPoint pos[4], vel[4], force[4];
        pos[0] = p.position; vel[0] = p.velocity; force[0] = QPoint();
        pos[1] = pos[0] + h/2 * vel[0]; vel[1] = vel[0] + h/2 * force[0]; force[1] = QPoint();
        pos[2] = pos[0] + h/2 * vel[1]; vel[1] = vel[0] + h/2 * force[1]; force[2] = QPoint();
        pos[3] = pos[0] + h * vel[2]; vel[1] = vel[0] + h * force[2]; force[3] = QPoint();
        p.position += h * ((vel[0] + vel[3])/ 6 + (vel[1] + vel[2]) / 3);
        p.velocity += h * ((force[0] + force[3]) / 6 + (force[1] + force[2]) / 3);
    }
}

void SingularState::load_stream(QDataStream input)
{
    input >> particles;
}

void SingularState::save_stream(QDataStream output)
{
    output << particles;
}
