#include "graphics.h"
#include <QtCore/QtMath>

qreal rsigm(qreal value, qreal tangent, qreal limit)
{
    return value / (value / limit + 1 / tangent);
}

namespace Visual {

ArrowItem::ArrowItem(QGraphicsItem *parent) :
    QAbstractGraphicsShapeItem(parent), _limitLength(0)
{
    setVel(QPoint(0, 0));
}

QRectF ArrowItem::boundingRect() const
{
    QRectF br = p.boundingRect();
    qreal pw = pen().widthF();
    br.adjust(-pw/2, -pw/2, pw/2, pw/2);
    return t.mapRect(br);
}

void ArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setPen(pen());
    painter->setTransform(t, true);
    painter->drawPath(p);
    painter->restore();
}

void ArrowItem::setVel(const QPointF &velocity)
{
    QLineF vel(QPointF(), velocity);
    QPainterPath newp;
    t.reset();
    qreal l = vel.length();
    if (_limitLength)
        l = rsigm(l, 1, _limitLength);
    qreal arrowScale = rsigm(l, 1, 1);
    newp.lineTo(l, 0);
    newp.moveTo(0.8 * l, -0.2 * arrowScale);
    newp.lineTo(l, 0);
    newp.lineTo(0.8 * l, 0.2 * arrowScale);
    t.rotate(-vel.angle());
    p = newp;
}

QPainterPath ParticleView::birdy;

ParticleView::ParticleView(QGraphicsItem *parent) :
    QAbstractGraphicsShapeItem(parent)
{
    arrow = new ArrowItem(this);
    arrow->hide();
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
    setFlag(ItemIsSelectable);
}

ParticleView::~ParticleView()
{
}

QRectF ParticleView::boundingRect() const
{
    qreal penwidth = pen().widthF();
    QRectF rect = QRectF(-0.5 - penwidth/2,
                  -0.5 - penwidth/2,
                  1 + penwidth,
                  1 + penwidth);
    if (isSelected())
        rect.adjust(-0.05,-0.05, 0.05, 0.05);
    return rect;
}

void ParticleView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setBrush(Qt::NoBrush);
    if (isSelected())
    {
        QPen pen2 = pen();
        pen2.setStyle(Qt::DashLine);
        painter->setPen(pen2);
        painter->drawEllipse(QPointF(0, 0), 0.55, 0.55);
    }
    painter->setPen(pen());
    painter->drawEllipse(QPointF(0, 0), 0.5, 0.5);
    painter->setBrush(brush());
    if (_vel.length() < CUTOFF)
    {
        painter->drawEllipse(QPointF(0, 0), 0.2, 0.2);
    }
    else
    {
        painter->rotate(-_vel.angle());
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

void ParticleView::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    arrow->show();
    arrow->update();
}

void ParticleView::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    arrow->hide();
    arrow->update();
}


void Visual::ParticleView::setVel(const QPointF &velocity)
{
    _vel.setP2(velocity);
    arrow->setVel(velocity);
}

Scenery::Scenery(const Simulation::VelocityMap *m, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    QSize size = m->get_size();
    int x, y;
    for (x = 0; x < size.width(); x++)
    {
        for (y = 0; y < size.height(); y++)
        {
            ArrowItem *i = new ArrowItem(this);
            i->limitLength(0.5);
            i->setPos(x, y);
            i->setVel(m->Get_Velocity_At(x, y)/5);
        }
    }
}

void Scenery::setPen(const QPen &pen)
{
    auto children = childItems();
    for (QGraphicsItem *child : children)
    {
        auto arrow = qgraphicsitem_cast<ArrowItem*>(child);
        arrow->setPen(pen);
    }
}




}
