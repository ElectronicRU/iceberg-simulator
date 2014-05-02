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

private:
    QList<SingularState> states;
    int cursor;
    qreal maxtime;
};
}

#endif // HISTORY_H
