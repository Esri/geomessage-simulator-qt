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
QMAKE_TARGET_COPYRIGHT = Copyright 2010-2012 Esri Inc.

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = ArcGISQtSearchAPIAppd
}
else{
    TARGET = ArcGISQtSearchAPIApp
}

INCLUDEPATH += $(ARCGISRUNTIMESDKQT_10_2)/sdk/include

win32{

  contains(QMAKE_TARGET.arch, x86_64):{
     message( "Building for Windows 64 bit")
     LIBS +=  \
       -L"$$(ARCGISRUNTIMESDKQT_10_2)"/ArcGISRuntime10.2/Client64
  }
  else {
     message( "Building for Windows 32 bit")
     LIBS +=  \
       -L"$$(ARCGISRUNTIMESDKQT_10_2)"/ArcGISRuntime10.2/Client32
  }

  CONFIG(debug, debug|release) {
    LIBS += \
            -lEsriRuntimeQtd
  } else {
    LIBS += \
            -lEsriRuntimeQt
  }
}

unix{
  QMAKE_CXXFLAGS += --std=c++0x

  LIBS +=  \
            -L$(ARCGISRUNTIMESDKQT_10_2)/ArcGISRuntime10.2/ClientLx \
            -lEsriRuntimeQt
}

win32:DEFINES += WINDOWS

SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

