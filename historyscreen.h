#ifndef HISTORYSCREEN_H
#define HISTORYSCREEN_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QAbstractGraphicsShapeItem>
#include <QPainter>
#include <QTimer>
#include "history.h"


namespace Visual {
using Simulation::History;
const qreal SCALE = 20.0;
class ParticleView : public QAbstractGraphicsShapeItem
{
public:
    ParticleView(QGraphicsItem *parent = 0);
    ~ParticleView();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QPainterPath shape() const;
    void setVel(const QPointF &velocity);
    QPointF vel()
    {
        return _vel.p2();
    }
    void setPen(const QPen &pen);

private:
    QLineF _vel;
    QGraphicsPathItem *arrow;
    static QPainterPath birdy;
    const qreal CUTOFF = 0.1;
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

class HistoryScreen : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HistoryScreen(QWidget *parent = 0);

    History *get_history() const;
    void set_history(History *value);

signals:
    void at(qreal time);
    void paused();
    void maxtimeChanged(qreal maxtime);

public slots:
    void playpause();
    void stop();
    void seek(qreal time);
    void seek_home();
    void seek_end();

private:
    History *history;
    QGraphicsScene *scene;
    QList<ParticleView*> particle_views;
    void pause();

    void update_scene(qreal time);

    qreal starttime;
    QTimer timer;

    const int period = 10; //msecs

private slots:
    void timer_tick();
};
}

#endif // HISTORYSCREEN_H
