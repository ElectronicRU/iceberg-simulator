#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QtMath>

using namespace Visual;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeSlider->setPageStep(qRound(STEP/QUANTUM));
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

void MainWindow::seekTime(int sliderValue)
{
    if (sliderValue == sliderValueCache)
        return;
    ui->historyScreen->seek(sliderValue * QUANTUM);
}

void MainWindow::moveSlider(qreal screenTime)
{
    sliderValueCache = qRound(screenTime / QUANTUM);
    ui->timeSlider->setValue(sliderValueCache);
}

void MainWindow::untogglePlay()
{
    ui->actionPlay->setChecked(false);
}

void MainWindow::setLimits(qreal maxtime)
{
    int maxticks = qCeil(maxtime / QUANTUM);
    ui->timeSlider->setRange(0, maxticks);
}

