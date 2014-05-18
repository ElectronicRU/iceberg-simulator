#include "historyscreen.h"
#include <QtCore/QtMath>
#include <QPainter>
using namespace Visual;

HistoryScreen::HistoryScreen(QWidget *parent) :
    QGraphicsView(parent), history(nullptr),
    scene(new QGraphicsScene(this)),
    starttime(0), timer(this)
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    QTransform m;
    m.scale(SCALE, -SCALE);
    setTransform(m);
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(period);
    connect(&timer, &QTimer::timeout, this, &HistoryScreen::timer_tick);
}
History *HistoryScreen::get_history() const
{
    return history;
}

void HistoryScreen::set_history(History *value)
{
    delete history;
    history = value;
    scene->clear();
    particle_views.clear();
    for (int i = 0; i < history->size(); i++)
    {
        ParticleView *v = new ParticleView();
        QBrush brush(Qt::red);
        if (i % 2)
            brush.setColor(Qt::green);
        v->setPen(QPen(brush, 0));
        v->setBrush(brush);
        Simulation::Particle p = history->at(i);
        v->setPos(p.position);
        v->setVel(p.velocity);
        scene->addItem(v);
        particle_views.append(v);
    }
    scene->setSceneRect(QRectF(QPointF(),
                        history->get_velocity_map()->get_size()));
    update_scene(0);
    emit maxtimeChanged(history->getMaxtime());
}

void HistoryScreen::playpause()
{
    if (timer.isActive())
    {
        timer.stop();
    }
    else
    {
        timer.start();
    }
}

void HistoryScreen::stop()
{
    timer.stop();
    update_scene(starttime);
}

void HistoryScreen::seek(qreal time)
{
    update_scene(time);
    starttime = history->tell();
    pause();
}

void HistoryScreen::seek_home()
{
    seek(0);
}

void HistoryScreen::seek_end()
{
    seek(history->getMaxtime());
}

void HistoryScreen::pause()
{
    timer.stop();
    emit paused();
}

void HistoryScreen::timer_tick()
{
    qreal time = history->tell();
    time += period / 1000.0;
    update_scene(time);
    if (history->tell() != time)
    {
        pause();
    }
}

void HistoryScreen::update_scene(qreal time)
{
    if (!history->seek(time))
        return;
    for (int i = 0; i < history->size(); i++)
    {
        ParticleView *v = particle_views[i];
        Simulation::Particle p = history->at(i);
        v->setPos(p.position);
        v->setVel(p.velocity);
    }
    emit at(time);
    update();
}

// ***************************************************************


QPainterPath ParticleView::birdy;

ParticleView::ParticleView(QGraphicsItem *parent) :
    QAbstractGraphicsShapeItem(parent)
{
    QPainterPath p;
    p.moveTo(0, 0);
    p.lineTo(1, 0);
    p.moveTo(0.75, 0.25);
    p.lineTo(1, 0);
    p.lineTo(0.75, -0.25);
    arrow = new QGraphicsPathItem(p, this);
    arrow->hide();
    arrow->setBrush(Qt::NoBrush);
    setAcceptHoverEvents(true);
    if (birdy.isEmpty())
    {
        qreal k = qSqrt(0.5);
        birdy.moveTo(0.5, 0);
        birdy.lineTo(-0.5 * k, 0.5 * k);
        birdy.lineTo(0, 0);
        birdy.lineTo(-0.5 * k, -0.5 * k);
        birdy.lineTo(0.5, 0);
    }
}

ParticleView::~ParticleView()
{
}

QRectF ParticleView::boundingRect() const
{
    qreal penwidth = pen().widthF();
    return QRectF(-0.5 - penwidth/2,
                  -0.5 - penwidth/2,
                  1 + penwidth,
                  1 + penwidth);
}

void ParticleView::paint(QPainter *painter, const QStyleOptionGraphicsItem *i, QWidget *w)
{
    painter->save();
    painter->setPen(pen());
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QPointF(0, 0), 0.5, 0.5);
    painter->setBrush(brush());
    if (_vel.length() < CUTOFF)
    {
        painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);
    }
    else
    {
        painter->drawPath(birdy);
    }
    painter->restore();
}

QPainterPath ParticleView::shape() const
{
    QPainterPath p;
    p.addEllipse(boundingRect());
    return p;
}

void ParticleView::setPen(const QPen &pen)
{
    QAbstractGraphicsShapeItem::setPen(pen);
    arrow->setPen(pen);
}

void ParticleView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    arrow->show();
    arrow->update();
}

void ParticleView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    arrow->hide();
    arrow->update();
}


void Visual::ParticleView::setVel(const QPointF &velocity)
{
    _vel.setP2(velocity);
    arrow->setScale(_vel.length());
    this->setRotation(-_vel.angle()); // ccw in QLineF, but cw in setRotation
}
