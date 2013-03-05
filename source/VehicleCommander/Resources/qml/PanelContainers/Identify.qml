import Qt 4.7
import "../Controls"
import "../Panels/Search"

Item
{
  signal closeMenu()
  signal exitClicked()
//  signal receiveBuddyPositionsClicked(bool state)
//  signal showBuddiesClicked(bool state)
//  signal receiveSpotReportsClicked(bool state)
//  signal showObservationsClicked(bool state)
//  signal showMeToggled(bool state)
//  signal sendMyPositionToggled(bool state)
//  signal resetMapClicked()
//  signal aboutMeClicked()
//  signal openCOA()
//  signal openMPK()

  property Stack stack
  property Panel panel

  function handleResultSelected(resultsList) {
    identifyResultsPanel.resultList = resultsList
  }

  id: identify
  objectName: "identify"
  panel: identifyResultsPanel

//  IdentifyPanel
//  {
//    id: identifyPanel;
//    anchors.fill: parent
//    stack: identify.stack;
//    visible: false
//    onGoBack: identify.closeMenu()
//    Component.onCompleted:
//    {
//      stack.addPanel(identifyPanel);
//      identifyPanel.exitClicked.connect(identify.exitClicked);
//    }
//  }

  ResultsPanel
  {
    id: identifyResultsPanel
    stack: identify.stack
    Component.onCompleted: resultSelected.connect(identify.handleResultSelected)
  }

  //  Reports
//  {
//    id:reports
//    stack: mainMenu.stack
//    anchors.fill: parent
//  }

//  OverlaysPanel
//  {
//    id:overlaysPanel
//    stack:mainMenu.stack
//    anchors.fill: parent
//  }

//  BuddiesPanel
//  {
//    id: buddiesPanel
//    stack: mainMenu.stack
//    anchors.fill: parent
//    Component.onCompleted:
//    {
//      receiveBuddyPositionsClicked.connect(mainMenu.receiveBuddyPositionsClicked)
//      showBuddiesClicked.connect(mainMenu.showBuddiesClicked)
//    }
//  }

//  ObservationsPanel
//  {
//    id: observationsPanel
//    stack: mainMenu.stack
//    anchors.fill: parent
//    Component.onCompleted:
//    {
//      receiveSpotReportsClicked.connect(mainMenu.receiveSpotReportsClicked)
//      showObservationsClicked.connect(mainMenu.showObservationsClicked)
//    }
//  }

//  OptionsPanel
//  {
//    id: optionsPanel
//    stack: mainMenu.stack
//    anchors.fill: parent
//    Component.onCompleted:
//    {
//      showMeToggled.connect(mainMenu.showMeToggled)
//      sendMyPositionToggled.connect(mainMenu.sendMyPositionToggled)
//      resetMapClicked.connect(mainMenu.resetMapClicked)
//      aboutMeClicked.connect(mainMenu.aboutMeClicked)
//    }
//  }
}
