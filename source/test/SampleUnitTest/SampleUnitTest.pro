QT       += xml testlib

QT       -= gui

TARGET = tst_sampleunittesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../VehicleCommander

SOURCES += tst_sampleunittesttest.cpp \
    ../../VehicleCommander/Geomessage.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../VehicleCommander/Geomessage.h
