#ifndef HISTORYSCREEN_H
#define HISTORYSCREEN_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QAbstractGraphicsShapeItem>
#include <QPainter>
#include <QTimer>
#include <QAction>
#include <QMouseEvent>
#include "history.h"
#include "graphics.h"


namespace Visual {
using Simulation::History;
const qreal SCALE = 20.0;

class HistoryScreen : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HistoryScreen(QWidget *parent = 0);
    ~HistoryScreen();

    History *get_history() const;
    void set_history(History *value);
    void setEditAction(QAction *action);

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
    void delete_selected();

private:
    History *history;
    QGraphicsScene *scene;
    QList<ParticleView*> particle_views;
    QAction *edit_action = 0;
    bool in_edit_mode = false;
    ParticleView *editing_it = 0;

    void pause();

    void update_scene(qreal time);

    qreal starttime;
    QTimer timer;

    const int period = 10; //msecs

private slots:
    void timer_tick();
    void edit_mode(bool mode);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
}

#endif // HISTORYSCREEN_H
