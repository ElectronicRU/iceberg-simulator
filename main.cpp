#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    VelocityMap *map = new VelocityMap(19,2,true);
    
/*
  Проверка:
    cout << map->GetNumber(0,0) << " " << map->GetNumber(0,1) << "\n" << map->GetNumber(1,0) << " " << map->GetNumber(1,1) << "\n" << map->GetNumber(2,0) << " " <<
            map->GetNumber(2,1) << "\n\n" << map->GetNumber(3,0) << " " << map->GetNumber(3,1) << "\n" << map->GetNumber(4,0) << " " << map->GetNumber(4,1) << "\n\n" <<
            map->GetNumber(5,0) << " " << map->GetNumber(5,1) << "\n" << map->GetNumber(6,0) << " " << map->GetNumber(6,1) << "\n\n" << map->GetNumber(7,0) << " " <<
            map->GetNumber(7,1) << "\n" << map->GetNumber(8,0) << " " << map->GetNumber(8,1) << "\n\n" << map->GetNumber(9,0) << " " << map->GetNumber(9,1) << "\n\n\n" <<
            map->GetNumber(17,0) << " " << map->GetNumber(17,1) << "\n" << map->GetNumber(18,0) << " " << map->GetNumber(18,1) << "\n\n" << endl;

    map->Save_Stream(Save_Vel);

    cout << (map->Get_Velocity_At(0.5,0.5))[0]<< "\n"<< (map->Get_Velocity_At(0.5,0.5))[1]<<endl;

    cout << (map->Calculate_Force(0.5,0.5))[0]<< "\n"<< (map->Calculate_Force(0.5,0.5))[1]<<endl;
*/

    return a.exec();
    delete map;
}
