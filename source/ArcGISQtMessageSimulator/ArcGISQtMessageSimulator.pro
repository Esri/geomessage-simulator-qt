QT       += network xml

CONFIG(debug, debug|release){
    TARGET = ArcGISQtMessageSimulatord
}
else{
    TARGET = ArcGISQtMessageSimulator
}
TEMPLATE = app


SOURCES += main.cpp\
        SimulatorDialog.cpp

HEADERS  += SimulatorDialog.h

FORMS    += SimulatorDialog.ui


OTHER_FILES += CopyReleaseFiles.bat \
               CopyDebugFiles.bat

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

RESOURCES +=

