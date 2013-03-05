QT       += network xml

TEMPLATE = app

CONFIG(debug, debug|release){
    TARGET = ArcGISQtMessageSimulatord
}
else{
    TARGET = ArcGISQtMessageSimulator
}

SOURCES += main.cpp\
        SimulatorDialog.cpp

HEADERS  += SimulatorDialog.h

FORMS    += SimulatorDialog.ui
