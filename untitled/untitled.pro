#-------------------------------------------------
#
# Project created by QtCreator 2013-10-07T18:12:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vpdataset.cpp \
    vpdatasource.cpp \
    ../data/vpparsertorque.cpp \
    ../data/vpparserserialbluetooth.cpp \
    ../data/vpparserserial.cpp \
    ../data/vpparsernmea.cpp \
    ../data/vpparsermindflex.cpp \
    ../data/vpparsercsv.cpp \
    ../data/vpparser.cpp

HEADERS  += mainwindow.h \
    vpdataset.h \
    vpdatasource.h \
    ../data/vpparsertorque.h \
    ../data/vpparserserialbluetooth.h \
    ../data/vpparserserial.h \
    ../data/vpparsernmea.h \
    ../data/vpparsermindflex.h \
    ../data/vpparsercsv.h \
    ../data/vpparser.h

FORMS    += mainwindow.ui
