QT       += opengl xml network declarative

TEMPLATE  = app
QMAKE_TARGET_COMPANY = Esri, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Runtime for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Runtime for Qt development.
QMAKE_TARGET_COPYRIGHT = Copyright 2010-2013 Esri Inc.

QMAKE_TARGET.arch = $$(BUILD_ARCH)

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = VehicleCommanderd
}
else{
    TARGET = VehicleCommander
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
           GPSSimulator.cpp \
           MapController.cpp \
           MainView.cpp \
           AppConfigDialog.cpp \
           DictionaryImageProvider.cpp \
           SearchController.cpp \
           simplegraphicoverlay.cpp \
           Geomessage.cpp

HEADERS  += GPSSimulator.h \
            MapController.h \
            MainView.h \
            AppConfigDialog.h \
            DictionaryImageProvider.h \
            SearchController.h \
            simplegraphicoverlay.h \
            Geomessage.h

RESOURCES += MainViewResources.qrc

FORMS += \
    AppConfigDialog.ui

OTHER_FILES += \
    Resources/ArcGISRuntimeLicense.txt \
    Resources/qml/MainOverlay.qml \
    Resources/qml/CompositeControls/StatusBar.qml \
    Resources/qml/CompositeControls/NumberPad.qml \
    Resources/qml/CompositeControls/NavigationControl2.qml \
    Resources/qml/CompositeControls/NavigationControl.qml \
    Resources/qml/CompositeControls/ChemLightBar.qml \
    Resources/qml/Controls/StackObject.js \
    Resources/qml/Controls/ToggleButton.qml \
    Resources/qml/Controls/Stack.qml \
    Resources/qml/Controls/ScrollButton.qml \
    Resources/qml/Controls/PushButton.qml \
    Resources/qml/Controls/Panel.qml \
    Resources/qml/Controls/MultiDelegate.qml \
    Resources/qml/Controls/ModelObject.qml \
    Resources/qml/Controls/Menu.qml \
    Resources/qml/Controls/ClassificationBar.qml \
    Resources/qml/PanelContainers/SpotReport.qml \
    Resources/qml/PanelContainers/Search.qml \
    Resources/qml/PanelContainers/Reports.qml \
    Resources/qml/PanelContainers/MainMenu.qml \
    Resources/qml/PanelContainers/Keywords.qml \
    Resources/qml/PanelContainers/EquipmentSearch.qml \
    Resources/qml/PanelContainers/Browse.qml \
    Resources/qml/Panels/MainMenu/ReportsPanel.qml \
    Resources/qml/Panels/MainMenu/OverlaysPanel.qml \
    Resources/qml/Panels/MainMenu/OptionsPanel.qml \
    Resources/qml/Panels/MainMenu/ObservationsPanel.qml \
    Resources/qml/Panels/MainMenu/MainPanel.qml \
    Resources/qml/Panels/MainMenu/BuddiesPanel.qml \
    Resources/qml/Panels/Search/TextSearchPanel.qml \
    Resources/qml/Panels/Search/SearchPanel.qml \
    Resources/qml/Panels/Search/ResultsPanel.qml \
    Resources/qml/Panels/Search/RecentPanel.qml \
    Resources/qml/Panels/Search/KeywordsPanel.qml \
    Resources/qml/Panels/Search/CategoryPanel.qml \
    Resources/qml/Panels/Search/BrowsePanel.qml \
    Resources/qml/Panels/Search/AffiliationPanel.qml \
    Resources/qml/Panels/Search/AddKeywordsPanel.qml \
    Resources/qml/Panels/SpotReport/UnitPanel.qml \
    Resources/qml/Panels/SpotReport/TimePanel.qml \
    Resources/qml/Panels/SpotReport/SpotReportPanel.qml \
    Resources/qml/Panels/SpotReport/SizePanel.qml \
    Resources/qml/Panels/SpotReport/LocationPanel.qml \
    Resources/qml/Panels/SpotReport/EquipmentPanel.qml \
    Resources/qml/Panels/Identify/IdentifyResultsPanel.qml \
    Resources/qml/Panels/SpotReport/ActivityPanel.qml
