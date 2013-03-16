#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T18:31:07
#
#-------------------------------------------------

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
