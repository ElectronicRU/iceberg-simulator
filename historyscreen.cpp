#include "historyscreen.h"
#include <QPainter>
using namespace Visual;

HistoryScreen::HistoryScreen(QWidget *parent) :
    QGraphicsView(parent), history(nullptr),
    scene(new QGraphicsScene(this)),
    starttime(0), timer(this)
{
    setScene(scene);
    setDragMode(RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    QTransform m;
    m.scale(SCALE, -SCALE);
    setTransform(m);
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(period);
    connect(&timer, &QTimer::timeout, this, &HistoryScreen::timer_tick);
}

HistoryScreen::~HistoryScreen()
{
    delete scene;
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

void HistoryScreen::setEditAction(QAction *action)
{
    edit_action = action;
    connect(action, SIGNAL(triggered(bool)), this, SLOT(edit_mode(bool)));
    if (history && !history->tell())
        action->setEnabled(true);
    else
        action->setEnabled(false);
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

void HistoryScreen::delete_selected()
{
    auto selected = QSet<QGraphicsItem*>::fromList(scene->selectedItems());
    QList<int> positions;
    int ito = 0;
    for (int i = 0; i < particle_views.size(); i++)
    {
        auto item = qgraphicsitem_cast<QGraphicsItem *>(particle_views[i]);
        particle_views[ito] = particle_views[i];
        if (selected.contains(item))
        {
            scene->removeItem(item);
            delete particle_views[i]; particle_views[i] = 0;
            positions.append(i);
        }
        else
        {
            ito++;
        }
    }
    history->remove_particles(positions);
    particle_views.erase(particle_views.begin() + ito, particle_views.end());
    update();
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

void HistoryScreen::edit_mode(bool mode)
{
    if (mode != in_edit_mode)
    {
        in_edit_mode = mode;
        edit_action->setChecked(mode);
    }
}

void HistoryScreen::update_scene(qreal time)
{
    if (edit_action)
        edit_action->setEnabled(time == 0);
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


void HistoryScreen::mousePressEvent(QMouseEvent *event)
{
    if (in_edit_mode)
    {
        QPointF realpos = mapToScene(event->pos());
        QBrush brush(Qt::blue);
        QPen pen(brush, 0);
        editing_it = new ParticleView();
        editing_it->setBrush(brush);
        editing_it->setPen(pen);
        editing_it->setPos(realpos);
        editing_it->setVel(history->get_velocity_map()->Get_Velocity_At(realpos.x(), realpos.y()));
        scene->addItem(editing_it);
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void HistoryScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (editing_it)
    {
        QPointF realpos = mapToScene(event->pos());
        editing_it->setPos(realpos);
        editing_it->setVel(history->get_velocity_map()->Get_Velocity_At(realpos.x(), realpos.y()));
        editing_it->update();
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void HistoryScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (editing_it)
    {
        QPointF realpos = mapToScene(event->pos());
        history->add_particle(realpos);
        particle_views.append(editing_it);
        editing_it = 0;
        update_scene(0);
        edit_mode(false);
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}


// ***************************************************************
