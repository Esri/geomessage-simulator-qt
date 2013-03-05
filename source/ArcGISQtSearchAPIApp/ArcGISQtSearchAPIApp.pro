#-------------------------------------------------
#
# Project created by QtCreator 2011-01-10T14:13:12
#
#-------------------------------------------------

QT       += core gui xml sql

TEMPLATE = app

QMAKE_TARGET_COMPANY = Esri, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Runtime for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Runtime for Qt development.
QMAKE_TARGET_COPYRIGHT = Copyright �2010-2012 Esri Inc. 

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

