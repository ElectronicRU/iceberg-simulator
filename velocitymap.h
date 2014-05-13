#ifndef VELOCITYMAP_H
#define VELOCITYMAP_H

#include "core.h"
#include <QSize>
#include <QString>

namespace Simulation {
class VelocityMap
{
private:

    unsigned int XIndex;   // размеры массива - строки
    unsigned int YIndex;   //                 - столбцы
    double **Pos_Vel;   // массив, где будем хранить карту

public:

    VelocityMap(unsigned int n,unsigned int m, bool IsFromFile);     // устанавливаем размер массива n x m, говорим: с файла ли данные
    ~VelocityMap();                                                 // деструктор

    double GetNumber(unsigned int n, unsigned int m);                // дает Pos_Vel[n][m]
    void SetNumber(unsigned int n, unsigned int m, double Var);     // устанавливает Pos_Vel[n][m] = Val

    void Load_Stream(QString FileName);                               // загрузка   с файла FileName
    void Save_Stream(QString FileName);                              // сохранение в

    QSize get_size();

    QPointF Get_Velocity_At(double x, double y);                    // интерполяция
    QPointF Calculate_Force(double x, double y);                    // расчет силы давления воды в точке (x,y)

};
}

#endif // VELOCITYMAP_H
