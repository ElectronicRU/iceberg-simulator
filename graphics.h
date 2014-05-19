#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QPainter>
#include <QAbstractGraphicsShapeItem>
#include "velocitymap.h"

namespace Visual {

class ArrowItem : public QAbstractGraphicsShapeItem
{
public:
    explicit ArrowItem(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setVel(const QPointF &velocity);
    void limitLength(qreal limit) { _limitLength = limit; }

private:
    QPainterPath p;
    QTransform t;
    qreal _limitLength;
};

class ParticleView : public QAbstractGraphicsShapeItem
{
public:
    explicit ParticleView(QGraphicsItem *parent = 0);
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
    ArrowItem *arrow;
    static QPainterPath birdy;
    const qreal CUTOFF = 0.1;
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
};

class Scenery: public QGraphicsItem
{
public:
    explicit Scenery(const Simulation::VelocityMap *m, QGraphicsItem *parent = 0);
    void setPen(const QPen &pen);
    QRectF boundingRect() const { return QRectF(); }
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) {}
};

}
#endif // GRAPHICS_H
