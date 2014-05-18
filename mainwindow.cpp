#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "velocitymap.h"
#include <QtCore/QtMath>

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QToolButton>
#include <qevent.h>

#include <qwt.h>

using namespace Visual;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionLoadMap, SIGNAL(triggered()), this, SLOT(loadMap()));
    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionSaveFile, SIGNAL(triggered()), this, SLOT(saveFile()));

    ui->actionLoadMap->setShortcut(QString("Ctrl+M"));
    ui->actionOpenFile->setShortcut(QString("Ctrl+O"));
    ui->actionSaveFile->setShortcut(QString("Ctrl+S"));

    updateActions();
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


void MainWindow::updateActions()
{
    ui->actionLoadMap->setEnabled(true);
    ui->actionOpenFile->setEnabled(true);
    ui->actionSaveFile->setEnabled(true);
}


void MainWindow::loadMap()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Load Velocity map"),
                                                    "",
                                                    tr("Docs(*.txt *.doc)"));
    if (FileName.isEmpty())
        return;

    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                            tr("File error"),
                            tr("Failed to open velocity map\n%1").arg(FileName));
        return;

    }

    Simulation::VelocityMap map(243,2,true);
    map.setFileName(FileName);
    map.Load_Stream(FileName);

    QMessageBox::information(this, "Successful opening",
                             "The Velocity Map was successfully opened");
}

void MainWindow::saveFile()
{
    Simulation::VelocityMap map(243,2,true);

        QString FileName = map.FileName();

        if (FileName.isEmpty())
            FileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                                    tr("Docs(*.txt)"));        //XXX нужно поставить нужное разрешение
        QFile file(FileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this,
                                tr("File error"),
                                tr("Failed to save\n%1").arg(FileName));
            map.setFileName(QString());
        } else {

            map.Save_Stream(FileName); // XXX здесь нужно поменять FileName на сохранение данных расчета, пока для проверки сохраняется карта скоростей
            QMessageBox::information(this, "Successful saving",
                                     "The data was successfully recorded");
            map.setFileName(FileName);



        }
file.close();
}

void MainWindow::openFile()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Docs(*.txt)"));        //XXX нужно поставить нужное разрешение
    if (FileName.isEmpty())
        return;

    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                            tr("File error"),
                            tr("Failed to open file\n%1").arg(FileName));
        return;

    }

/* XXX нужна новая функция load - что именно загружаем
         QMessageBox::information(this, "Successful opening",
                                 "The file was successfully opened");
*/
}







