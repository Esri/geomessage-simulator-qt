QMAKE_TARGET_COMPANY = Esri, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Runtime for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Runtime for Qt development.
QMAKE_TARGET_COPYRIGHT = Copyright 2010-2013 Esri Inc.

QT += core gui opengl

TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

# Important: requires file: $qtsdk\mkspecs\features\esri_runtime_qt_10_2.prf
# See ArcGIS Runtime Qt SDK documentation for more information
CONFIG += esri_runtime_qt_10_2

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = MapTestAppd
}
else{
    TARGET = MapTestApp
}

win32:DEFINES += WINDOWS

win32:CONFIG += \
  embed_manifest_exe

SOURCES += main.cpp\
        mainwindow.cpp \
    mapcompass.cpp \
    mapscalebar.cpp \
    pages.cpp \
    configdialog.cpp \
    messagereaderxml.cpp \
    samplemap.cpp

HEADERS  += mainwindow.h \
    mapcompass.h \
    mapscalebar.h \
    pages.h \
    configdialog.h \
    messagereaderxml.h \
    samplemap.h


FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
