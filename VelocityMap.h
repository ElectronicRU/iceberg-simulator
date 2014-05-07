#ifndef VELOCITYMAP_H
#define VELOCITYMAP_H

#include "service.h"
#include <string>

using namespace std;

class VelocityMap
{
private:

    unsigned int XIndex;   // размеры массива - строки
    unsigned int YIndex;   //                 - столбцы
    double **Velocities;   // массив, где будем хранить карту

public:

    VelocityMap(unsigned int n,unsigned int m, bool IsFromFile);
    ~VelocityMap();

    double GetNumber(unsigned int n, unsigned int m);
    void SetNumber(unsigned int n, unsigned int m, double Var);

    double* operator[](unsigned int n);

    void Load_Stream(string FileName);
    void Save_Stream(string FileName);

    double* Get_Velocity_At(double x, double y);
    double* Calculate_Force(double x, double y);

};

#endif // VELOCITYMAP_H
