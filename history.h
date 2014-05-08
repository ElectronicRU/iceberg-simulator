#ifndef HISTORY_H
#define HISTORY_H
#include "singularstate.h"
#include <QList>

namespace Simulation {
class History
{
public:
    History(SingularState);
    void load_stream(QDataStream input);
    void save_stream(QDataStream output);
    void continue_history(qreal time);
    void seek(qreal time);
    qreal tell();
    void add_particles(QList<QPointF> points);
    void remove_particles(QList<int> positions);
private:
    QList<SingularState> states;
    int cursor;
    qreal maxtime;
    void chain_update();
};
}

#endif // HISTORY_H
