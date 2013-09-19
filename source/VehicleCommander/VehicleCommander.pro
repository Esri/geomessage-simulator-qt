QMAKE_TARGET_COMPANY = Esri, Inc.
QMAKE_TARGET_PRODUCT = ArcGIS Runtime for Qt
QMAKE_TARGET_DESCRIPTION = Sample application using ArcGIS Runtime for Qt development.
QMAKE_TARGET_COPYRIGHT = Copyright 2010-2013 Esri Inc.

QT       += opengl xml network declarative

TEMPLATE  = app

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

# Important: requires file: $qtsdk\mkspecs\features\esri_runtime_qt_10_2.prf
# See ArcGIS Runtime Qt SDK documentation for more information
CONFIG += esri_runtime_qt_10_2

CONFIG += debug_and_release
CONFIG(debug, debug|release){
    TARGET = VehicleCommanderd
}
else{
    TARGET = VehicleCommander
}

win32:DEFINES += WINDOWS

win32:CONFIG += \
  embed_manifest_exe

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
