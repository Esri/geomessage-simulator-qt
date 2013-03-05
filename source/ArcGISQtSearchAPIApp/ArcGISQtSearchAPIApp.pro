#-------------------------------------------------
#
# Project created by QtCreator 2011-01-10T14:13:12
#
#-------------------------------------------------

QT       += core gui xml sql

TEMPLATE = app

VERSION = 1.0
QMAKE_TARGET_COMPANY = Environmental Systems Research Institute, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Mobile for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Mobile library for Qt/C++ development.
QMAKE_TARGET_COPYRIGHT = Copyright �2010-2012 ESRI Inc. All Rights Reserved.

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = ArcGISQtSearchAPIAppd
}
else{
    TARGET = ArcGISQtSearchAPIApp
}

unix{
    QMAKE_CXXFLAGS += --std=c++0x

  INCLUDEPATH += $(ARCGISRUNTIMESDKQT_10_2)/sdk/include

  LIBS +=  \
            -L$(ARCGISRUNTIMESDKQT_10_2)/ArcGISRuntime10.2/ClientLx \
            -lEsriRuntimeQt
}

win32:DEFINES += MSWINDOWS
unix:DEFINES  += LINUXx86

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

