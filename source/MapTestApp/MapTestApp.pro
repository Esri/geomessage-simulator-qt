#-------------------------------------------------
#
# Project created by QtCreator 2012-05-10T09:33:25
#
#-------------------------------------------------

QT += core gui opengl

TEMPLATE = app
VERSION = 1.0
QMAKE_TARGET_COMPANY = Environmental Systems Research Institute, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Mobile for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Mobile library for Qt/C++ development.
QMAKE_TARGET_COPYRIGHT = Copyright �2010-2012 ESRI Inc. All Rights Reserved.

QMAKE_TARGET.arch = $$(BUILD_ARCH)

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = MapTestAppd
}
else{
    TARGET = MapTestApp
}

win32{
  INCLUDEPATH += "$$ARCGIS_RUNTIME_DIR/include"

  #64 Bit Windows Build
  contains(QMAKE_TARGET.arch, x86_64):{
  message( "Building for 64 bit")
    CONFIG(debug, debug|release){
      LIBS  += -L"$$ARCGIS_RUNTIME_DIR/lib64" \
               -lArcGISQtd1 \
               -lSymbolDictionaryd1
    }
    else{
      LIBS  += -L"$$ARCGIS_RUNTIME_DIR/lib64" \
               -lArcGISQt1 \
               -lSymbolDictionary1
    }
  }

  #32 Bit Windows Build
  contains(QMAKE_TARGET.arch, x86):{
  message( "Building for 32 bit")
    CONFIG(debug, debug|release){
      LIBS  += -L"$$ARCGIS_RUNTIME_DIR/lib32" \
               -lArcGISQtd1 \
               -lSymbolDictionaryd1
    }
    else{
      LIBS  += -L"$$ARCGIS_RUNTIME_DIR/lib32" \
               -lArcGISQt1 \
               -lSymbolDictionary1

    }
  }
}

unix{
  QMAKE_CXXFLAGS += --std=c++0x

  INCLUDEPATH += $(ARCGISRUNTIMESDKQT_10_2)/sdk/include

  LIBS +=  \
            -L$(ARCGISRUNTIMESDKQT_10_2)/ArcGISRuntime10.2/ClientLx \
            -lEsriRuntimeQt
}

win32{
#in Windows, you can use & to separate commands
    copyfiles.commands += @echo NOW COPYING ADDITIONAL FILE(S) $${TARGET} &
    #copyfiles.commands += @call ..\\$${TARGET}\copyresources.bat
    copyfiles.commands += @call xcopy "..\\$${TARGET}\\Resources" "..\\MapIFTestApp-build-desktop\\Resources\\" /E /Y
}
QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

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
