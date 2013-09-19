QT       += core gui xml sql

TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

QMAKE_TARGET_COMPANY = Esri, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Runtime for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Runtime for Qt development.
QMAKE_TARGET_COPYRIGHT = Copyright 2010-2013 Esri Inc.

# Important: requires file: $qtsdk\mkspecs\features\esri_runtime_qt_10_2.prf
# See ArcGIS Runtime Qt SDK documentation for more information
CONFIG += esri_runtime_qt_10_2

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = ArcGISQtSearchAPIAppd
}
else{
    TARGET = ArcGISQtSearchAPIApp
}

win32:DEFINES += WINDOWS

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

