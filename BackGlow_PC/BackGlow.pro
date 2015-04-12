#-------------------------------------------------
#
# Project created by QtCreator 2014-12-28T18:56:27
#
#-------------------------------------------------

QT       += core gui serialport widgets

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = BackGlow
TEMPLATE = app

INCLUDEPATH += "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\include"

win32:LIBS += -lGdi32 -lUser32
unix:!macx:LIBS += -lX11

QMAKE_CXXFLAGS += -std=c++11

SOURCES +=  main.cpp\
            mainwindow.cpp \
            backglow.cpp \
            screencapture_.cpp

win32:SOURCES +=    windows/screencapture.cpp \
                    windows/serial.cpp
unix:!macx:SOURCES += linux/screencapture.cpp \
                      linux/serial.cpp


HEADERS  += mainwindow.h \
            backglow.h \
            screencapture_.h \
            serial_.h


win32:HEADERS +=    windows/screencapture.h \
                    windows/serial.h
unix:!macx:HEADERS += linux/screencapture.h \
                      linux/serial.h

FORMS    += mainwindow.ui



win32:LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v7.0/lib/x64/" -lOpenCL
