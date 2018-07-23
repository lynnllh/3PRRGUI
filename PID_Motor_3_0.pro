#-------------------------------------------------
#
# Project created by QtCreator 2016-08-04T13:07:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PID_Motor_2_0
TEMPLATE = app


SOURCES += main.cpp\
        pidwidget.cpp \
    axisonethread.cpp \
    axistwothread.cpp \
    axisfivethread.cpp \
    axisfourthread.cpp \
    axissixthread.cpp \
    axisthreethread.cpp

HEADERS  += pidwidget.h \
    gts.h \
    axisonethread.h \
    axistwothread.h \
    axisthreethread.h \
    axisfourthread.h \
    axisfivethread.h \
    axissixthread.h

FORMS    += pidwidget.ui
LIBS += gts.lib

RESOURCES += \
    icon.qrc
RC_FILE += ICON.rc
