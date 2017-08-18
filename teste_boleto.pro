#-------------------------------------------------
#
# Project created by QtCreator 2017-08-17T17:10:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = teste_boleto
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        network.cpp \
        network_controller.cpp

HEADERS  += mainwindow.h \
        network.h \
        network_controller.h


FORMS    += mainwindow.ui

