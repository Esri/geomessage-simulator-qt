QT       += network xml

CONFIG(debug, debug|release){
    TARGET = ArcGISQtMessageSimulatord
}
else{
    TARGET = ArcGISQtMessageSimulator
}
TEMPLATE = app


SOURCES += main.cpp\
        SimulatorDialog.cpp \
    SimulatorController.cpp \
    Geomessage.cpp

HEADERS  += SimulatorDialog.h \
    SimulatorController.h \
    Geomessage.h

FORMS    += SimulatorDialog.ui

win32{
  CONFIG(debug, debug|release) {
    CONFIG -= debug \
        release
    CONFIG += debug
  }
  CONFIG(release, debug|release) {
    CONFIG -= debug \
        release
    CONFIG += release
  }
}

unix{
  CONFIG(debug, debug|release){
  }
  else{
  }
}

win32:DEFINES += WINDOWS

RESOURCES +=

CONFIG += console
