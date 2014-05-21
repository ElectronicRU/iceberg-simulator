#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "velocitymap.h"
#include <QtCore/QtMath>

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QToolButton>
#include <qevent.h>

#include <QInputDialog>
#include <qwt.h>

using namespace Visual;
using Simulation::VelocityMap;
using Simulation::History;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->historyScreen->setEditAction(ui->actionAddParticle);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setHistory(History *history)
{
    ui->historyScreen->set_history(history);
    emit loaded(true);
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

void MainWindow::requestMaxtime()
{
    History *h = ui->historyScreen->get_history();
    qreal currmaxtime = h->getMaxtime();
    bool ok;
    qreal maxtime = QInputDialog::getDouble(this, "Edit maximum time", "Enter new maximum time", currmaxtime, 0, 1e5,
                                            1, &ok);
    if (ok)
    {
        h->setMaxtime(maxtime);
        setLimits(maxtime);
        ui->historyScreen->seek_home();
    }
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

    VelocityMap *map = VelocityMap::LoadText(&file);

    velmapFileName = FileName;
    setHistory(new History(map));

    QMessageBox::information(this, "Successful opening",
                             "The Velocity Map was successfully opened");
    file.close();
}

void MainWindow::saveFile()
{
    QString FileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                                    tr("Docs(*.sim)"));
    if (FileName.isEmpty())
        return;
    QFile file(FileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,
                            tr("File error"),
                            tr("Failed to open file\n%1").arg(FileName));
        return;
    }
    ui->historyScreen->get_history()->save_stream(&file);
    file.flush();
    file.close();
}

void MainWindow::openFile()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Docs(*.sim)"));

    if (FileName.isEmpty())
        return;

    QFile file(FileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,
                            tr("File error"),
                            tr("Failed to open file\n%1").arg(FileName));
        return;
    }

    History *h = new History();
    if (h->load_stream(&file))
        setHistory(h);
    else
        delete h;
    file.close();
}







