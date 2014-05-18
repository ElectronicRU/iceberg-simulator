#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "core.h"
#include "history.h"

namespace Ui {
class MainWindow;
}

namespace Visual {
using Simulation::QUANTUM;
using Simulation::STEP;
using Simulation::History;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setHistory(History *history);

public slots:
    void seekTime(double sliderValue);
    void moveSlider(qreal screenTime);
    void untogglePlay();
    void setLimits(qreal maxtime);

    void loadMap();
    void openFile();
    void saveFile();

private slots:
    void updateActions();

private:
    Ui::MainWindow *ui;
    double sliderValueCache;
};
}

#endif // MAINWINDOW_H
