#-------------------------------------------------
#
# Project created by QtCreator 2011-11-26T14:27:19
#
#-------------------------------------------------

QT       += core gui

TARGET = SliderMultiPos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CSliderMultiPos.cpp \
    cslidermp_handle.cpp \
    cslidermp_interval.cpp

HEADERS  += mainwindow.h \
    CSliderMultiPos.h \
    cslidermp_handle.h \
    cslidermp_interval.h

FORMS    += mainwindow.ui

mac{
    CONFIG -= app_bundle
    CONFIG += x86_64
}




