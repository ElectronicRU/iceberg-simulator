#include "historyscreen.h"
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
    QRectF rect = QRectF(QPointF(),
                               history->get_velocity_map()->get_size());
    rect.adjust(-1, -1, 1, 1);
    scene->setSceneRect(rect);
    fitInView(scene->sceneRect(),
              Qt::KeepAspectRatio);

    Scenery *scenery = new Scenery(history->get_velocity_map());
    scenery->setZValue(-1);
    scenery->setPen(QPen(Qt::blue, 0));
    scene->addItem(scenery);

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
