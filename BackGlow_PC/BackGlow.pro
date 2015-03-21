#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T18:56:27
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BackGlow
TEMPLATE = app

win32:LIBS += -lGdi32 -lUser32

SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp \
    backglow.cpp

HEADERS  += mainwindow.h \
    serial.h \
    backglow.h

FORMS    += mainwindow.ui
