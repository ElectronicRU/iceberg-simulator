#-------------------------------------------------
#
# Project created by QtCreator 2014-04-25T22:32:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iceberg-simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    singularstate.cpp \
    history.cpp \
    velocitymap.cpp

HEADERS  += mainwindow.h \
    singularstate.h \
    core.h \
    history.h \
    velocitymap.h

FORMS    += mainwindow.ui

RESOURCES += \
    mainwindow.qrc
