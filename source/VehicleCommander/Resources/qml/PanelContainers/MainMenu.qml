import QtQuick 1.1
import "../Panels/MainMenu"
import "../Controls"

Item
{
  signal closeMenu()
  signal exitClicked()
  signal receiveBuddyPositionsClicked(bool state)
  signal showBuddiesClicked(bool state)
  signal receiveSpotReportsClicked(bool state)
  signal showObservationsClicked(bool state)
  signal showMeToggled(bool state)
  signal followMeToggled(bool state)
  signal sendMyPositionToggled(bool state)
  signal resetMapClicked()
  signal aboutMeClicked()
  signal openCOA()
  signal openMPK()
  signal toggleLayerVisibility(string name)
  signal clicked()
  signal visibilityAnalysisClicked()

  property Stack stack
  property Panel panel

  id: mainMenu
  objectName: "mainMenu"
  panel: mainPanel

  MainPanel
  {
    id: mainPanel;
    reportsPanel: reports.panel
    overlaysPanel: overlaysPanel
    buddiesPanel: buddiesPanel
    observationsPanel: observationsPanel
    optionsPanel: optionsPanel
    visibilityPanel: visibilityPanel

    anchors.fill: parent
    stack: mainMenu.stack;
    visible: false
    onGoBack: mainMenu.closeMenu()
    Component.onCompleted:
    {
// WARNING: There is a known issue that when
// this project's QML is run in the QML debugger, it does initialize
// in the correct order and the following properties are null
// For now, you will need to disable the QML debugger to run the debugger
// as a workaround (in Projects | Run | Debugger Settings)
      stack.addPanel(mainPanel);
      mainPanel.exitClicked.connect(mainMenu.exitClicked);
      mainPanel.buttonClicked.connect(mainMenu.clicked)
    }
  }

  Reports
  {
    id:reports
    stack: mainMenu.stack
    anchors.fill: parent
  }

  OverlaysPanel
  {
    id:overlaysPanel
    stack:mainMenu.stack
    anchors.fill: parent
  }

  BuddiesPanel
  {
    id: buddiesPanel
    stack: mainMenu.stack
    anchors.fill: parent
    Component.onCompleted:
    {
      receiveBuddyPositionsClicked.connect(mainMenu.receiveBuddyPositionsClicked)
      showBuddiesClicked.connect(mainMenu.showBuddiesClicked)
    }
  }

  ObservationsPanel
  {
    id: observationsPanel
    stack: mainMenu.stack
    anchors.fill: parent
    Component.onCompleted:
    {
      receiveSpotReportsClicked.connect(mainMenu.receiveSpotReportsClicked)
      showObservationsClicked.connect(mainMenu.showObservationsClicked)
    }
  }

  OptionsPanel
  {
    id: optionsPanel
    stack: mainMenu.stack
    anchors.fill: parent
    Component.onCompleted:
    {
      showMeToggled.connect(mainMenu.showMeToggled)
      followMeToggled.connect(mainMenu.followMeToggled)
      sendMyPositionToggled.connect(mainMenu.sendMyPositionToggled)
      resetMapClicked.connect(mainMenu.resetMapClicked)
      aboutMeClicked.connect(mainMenu.aboutMeClicked)
    }
  }

  VisibilityPanel
  {
    id: visibilityPanel
    stack: mainMenu.stack
    anchors.fill: parent
    Component.onCompleted:
    {
      visibilityAnalysisClicked.connect(mainMenu.visibilityAnalysisClicked)
    }
  }
}
