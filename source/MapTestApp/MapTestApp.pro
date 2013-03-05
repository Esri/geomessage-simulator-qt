#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T09:33:25
#
#-------------------------------------------------

QT += core gui opengl

TEMPLATE = app
QMAKE_TARGET_COMPANY = Esri, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Runtime for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Runtime for Qt development.
QMAKE_TARGET_COPYRIGHT = Copyright �2010-2013 Esri Inc. 

QMAKE_TARGET.arch = $$(BUILD_ARCH)

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = MapTestAppd
}
else{
    TARGET = MapTestApp
}

unix{
  QMAKE_CXXFLAGS += --std=c++0x

  INCLUDEPATH += $(ARCGISRUNTIMESDKQT_10_2)/sdk/include

  LIBS +=  \
            -L$(ARCGISRUNTIMESDKQT_10_2)/ArcGISRuntime10.2/ClientLx \
            -lEsriRuntimeQt
}

win32:DEFINES += WINDOWS
unix:DEFINES  += LINUXx86

SOURCES += main.cpp\
        mainwindow.cpp \
    mapcompass.cpp \
    pages.cpp \
    configdialog.cpp \
    samplemap.cpp


HEADERS  += mainwindow.h \
    mapcompass.h \
    pages.h \
    configdialog.h \
    samplemap.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc
