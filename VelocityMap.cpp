#include "velocitymap.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "service.h"

using namespace std;

VelocityMap::VelocityMap(unsigned int n, unsigned int m, bool IsFromFile)
{
    unsigned int i;
    Velocities = new double* [n];
    for (i = 0; i < n; i++)
        Velocities[i] = new double [m];
    if (IsFromFile)
        this->Load_Stream(Load_Vel);
    XIndex = n;
    YIndex = m;
}


VelocityMap::~VelocityMap()
{
    unsigned int i;
    for (i = 0; i < XIndex; i++)
        delete [] Velocities[i];
}


void VelocityMap::Load_Stream (string FileName) // Функция загрузки из потока данных матрицы n x m
{
    double Var;                                  // переменные
    ifstream file;                              // поток чтения из файла

    unsigned int i, j;                         // i - столбцы, j - строки

    // Открытие файла
    file.open(FileName.c_str());

    // Проверка успешности открытия файла
    if (file.fail())
        {
            cout<<"\n Error of the file opening - Loading";
                   exit(1);
        }

    for (i=0; i < XIndex; i++)
    {
        for(j=0; j < YIndex; j++)
        {
            file >> Var;
            this->SetNumber(i, j, Var);
        }
    }

// Закрытие файла
    file.close();
}


void VelocityMap::Save_Stream(string FileName)   // Функция сохранения в поток данных
{
 double Var;
 ofstream file;                                            // поток записи в файл
 unsigned int i,j;

 file.open(FileName.c_str());

   if (file.fail())
   {
       cout<<"\n Error of the file opening - Saving";
       exit(1);
   }

  for(i=0; i < XIndex; i++)
   {
      file << endl;
      for(j=0; j < YIndex; j++)
      {
            Var = this->GetNumber(i, j);
            file << Var << endl;
      }
   }
 cout << "The data was recorded" << endl;
 file.close();
}


double VelocityMap::GetNumber(unsigned int n, unsigned int m)
{
    if (n < XIndex && m < YIndex)
        return Velocities[n][m];
    else
        return 30000000; // c
}

/* double* VelocityMap::operator [](unsigned int n)  // перегрузка операторов [][]
{
    if (n < XIndex)
                return Velocities[XIndex];
    return NULL;
}

*/

void VelocityMap::SetNumber(unsigned int n, unsigned int m, double Var)
{
    if (n < XIndex && m < YIndex)
        Velocities[n][m] = Var;
}


double* VelocityMap::Get_Velocity_At (double x, double y)            // Интерполяция

{
    //Хотим попасть в нужный квадрат

    int int_x = x;                                    // целая часть
    int int_y = y;

    Load_Stream(Load_Vel);                            // загружаем карту скоростей

    int n = GetNumber(0,0);                           // высота карты
    int m = GetNumber(0,1);                           // ширина
    int pos = int_x*(2*(n+1)) + 2*int_y + 2;         // положение скоростей нижнего левого угла(в файле)

    double x0 = int_x;                               // Q0 = (x0, y0) - левый нижний угол нашего квадрата
    double y0 = int_y;
    double Vx0 = GetNumber(pos,0);
    double Vy0 = GetNumber(pos,1);


    double x1 = x0 + 1;
    double y1 = y0 + 1;

    double Vx1;                                  // Q1 = (x0, y1)
    double Vy1;

    double Vx2;                                  // Q2 = (x0, y1)
    double Vy2;

    double Vx3;                                  // Q3 = (x1, y1)
    double Vy3;

    if (x0<m && y0<n)
{
    Vx1 = GetNumber(pos+2,0);
    Vy1 = GetNumber(pos+2,1);

    Vx2 = GetNumber(pos+2*(n+1),0);
    Vy2 = GetNumber(pos+2*(n+1),1);


    Vx3 = GetNumber(pos + 2*(n+1) + 2, 0);
    Vy3 = GetNumber(pos + 2*(n+1) + 2, 1);
}
    else
    {
        if (x0>m || x0<0 || y0>n || y0<0)
        {
        cout << "Вышли за границу карты";
        }

        if (y0 == n)                  // на верхней границе
        {
        Vx1 = 0;
        Vy1 = 0;
        Vx3 = 0;
        Vy3 = 0;
        }

        if (x0 == m)                 // на правой границе
        {
        Vx2 = 0;
        Vy2 = 0;
        Vx3 = 0;
        Vy3 = 0;
        }
    }

 // координаты нашей точки P(x,y) в этом квадрате

    // Интерполяция по х
    double VR1x = (x1-x)*Vx0 + (x-x0)*Vx2;               // R1(x,y1)     (x1-x0) = 1
    double VR1y = (x1-x)*Vy0 + (x-x0)*Vy2;

    double VR2x = (x1-x)*Vx1 + (x-x0)*Vx3;              // R2(x,y2)
    double VR2y = (x1-x)*Vy1 + (x-x0)*Vy3;

    // Интерполяция по у
    double Vx = (y1-y)*VR1x + (y-y0)*VR2x;             // (y1-y0) = 1
    double Vy = (y1-y)*VR1y + (y-y0)*VR2y;

    double V[2]= {Vx, Vy};
//    cout << Vx << " " << Vy << endl;
    return V;
}


double* VelocityMap::Calculate_Force (double x, double y)                      // Сила F = P*S; P = [ro*v^2]/2 - Bernulli; x, y нужно будет заменит на i - номер частицы(узнаем из SingularState)
{

    double* v;
    v = Get_Velocity_At(x,y);

    double vx = v[0];
    double vy = v[1];

    double F[2];
    F[0] = S*ro*sqrt(pow(vx,2)+pow(vy,2))*vx/2;                   // Fx
    F[1] = S*ro*sqrt(pow(vx,2)+pow(vy,2))*vy/2;                   // Fy

    return F;
}



