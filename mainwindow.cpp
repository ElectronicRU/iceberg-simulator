#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QtMath>
#include <qwt.h>

using namespace Visual;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setHistory(History *history)
{
    ui->historyScreen->set_history(history);
    ui->playbackToolBar->setEnabled(true);
}

void MainWindow::seekTime(double sliderValue)
{
    if (sliderValue == sliderValueCache)
        return;
    ui->historyScreen->seek(sliderValue);
}

void MainWindow::moveSlider(qreal screenTime)
{
    sliderValueCache = screenTime;
    ui->timeSlider->setValue(sliderValueCache);
}

void MainWindow::untogglePlay()
{
    ui->actionPlay->setChecked(false);
}

void MainWindow::setLimits(qreal maxtime)
{
    ui->timeSlider->setUpperBound(maxtime);
    ui->timeSlider->setTotalSteps(qRound(maxtime / QUANTUM));
    ui->timeSlider->setPageSteps(qRound(maxtime / STEP));
}

