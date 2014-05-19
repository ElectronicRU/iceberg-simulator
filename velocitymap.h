#ifndef VELOCITYMAP_H
#define VELOCITYMAP_H

#include "core.h"
#include <QSize>
#include <QString>
#include <QFile>

namespace Simulation {
class VelocityMap
{
friend class History;
private:

    unsigned int Size;   // размеры массива - строки
    double **Pos_Vel;   // массив, где будем хранить карту

public:

    VelocityMap(unsigned int n);     // устанавливаем размер массива n
    ~VelocityMap();                                                 // деструктор

    double GetNumber(unsigned int n, unsigned int m) const;                // дает Pos_Vel[n][m]
    void SetNumber(unsigned int n, unsigned int m, double Var);     // устанавливает Pos_Vel[n][m] = Val

    static VelocityMap* LoadText(QFile *file);                               // загрузка   с файла FileName
    static VelocityMap* load_stream(QDataStream &input);
    void save_stream(QDataStream &output);

    QSize get_size() const;

    QPointF Get_Velocity_At(double x, double y) const;                    // интерполяция
    QPointF Calculate_Force(double x, double y, QPointF V) const;                    // расчет силы давления воды в точке (x,y)

};
}

#endif // VELOCITYMAP_H
