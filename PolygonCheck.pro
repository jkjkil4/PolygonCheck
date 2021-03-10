#-------------------------------------------------
#
# Project created by QtCreator 2021-02-23T21:04:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PolygonCheck
TEMPLATE = app

RC_ICONS += PolygonCheck_.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Widget/CheckCircleWidget/checkcirclesidebar.cpp \
        Widget/CheckCircleWidget/checkcircleviewport.cpp \
        Widget/CheckCircleWidget/checkcirclewidget.cpp \
        Widget/CheckLineWidget/checklinesidebar.cpp \
        Widget/CheckLineWidget/checklineviewport.cpp \
        Widget/CheckLineWidget/checklinewidget.cpp \
        Widget/CheckPointWidget/checkpointsidebar.cpp \
        Widget/CheckPointWidget/checkpointviewport.cpp \
        Widget/CheckPointWidget/checkpointwidget.cpp \
        Widget/CheckPolygonWidget/checkpolygonsidebar.cpp \
        Widget/CheckPolygonWidget/checkpolygonviewport.cpp \
        Widget/CheckPolygonWidget/checkpolygonwidget.cpp \
        Widget/CheckPolygonWidget/editpolygonwidget.cpp \
        Widget/digitedit.cpp \
        main.cpp \
        widget.cpp

HEADERS += \
        Widget/CheckCircleWidget/checkcirclesidebar.h \
        Widget/CheckCircleWidget/checkcircleviewport.h \
        Widget/CheckCircleWidget/checkcirclewidget.h \
        Widget/CheckLineWidget/checklinesidebar.h \
        Widget/CheckLineWidget/checklineviewport.h \
        Widget/CheckLineWidget/checklinewidget.h \
        Widget/CheckPointWidget/checkpointsidebar.h \
        Widget/CheckPointWidget/checkpointviewport.h \
        Widget/CheckPointWidget/checkpointwidget.h \
        Widget/CheckPolygonWidget/checkpolygonsidebar.h \
        Widget/CheckPolygonWidget/checkpolygonviewport.h \
        Widget/CheckPolygonWidget/checkpolygonwidget.h \
        Widget/CheckPolygonWidget/editpolygonwidget.h \
        Widget/digitedit.h \
        header.h \
        widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($(Lib)/MyQtLibs/myqtlibs.pri)
