import Qt 4.7
// import QtQuick 1.0

import "CompositeControls"
import "PanelContainers"
import "Controls"
import "Panels/Identify"

LayoutItem
{
  signal basemapChanged(string name)
  signal homeClicked()
  signal chemLightSelected(string color)
  signal zoomInClicked()
  signal zoomOutClicked()
  signal panClicked(string direction)
  signal emergencyClicked(bool state)
  signal measureLineClicked(bool state)
  signal measureAreaClicked(bool state)
  signal measurePointAndRadiusClicked(bool state)

  //For fake values in updateSpeed and updateAltitude functions
  property int lastFakeSpeed: 60;
  property int lastFakeAltitude: 1814;

  function clicked()
  {
    mainMenu.clicked();
  }

  function updateLocation(newLocation)
  {
    statusBar.currentLocation = "Location: " + newLocation
  }

  function updateSpeed(newSpeed)
  {
    //TODO we should use the actual speed, as follows:
    //statusBar.currentSpeed = "   Speed: " + (Math.round(10.0 * newSpeed) / 10.0);
    //However, the app provides speeds that are unrealistically fast and highly variable.
    //Therefore, fake it as follows:
    var rand = Math.random();
    if (rand < 0.005)
      lastFakeSpeed--;
    else if (rand > 0.995)
      lastFakeSpeed++;
    statusBar.currentSpeed = "   Speed: " + lastFakeSpeed;
  }

  function updateAltitude(newAltitude)
  {
    //TODO we should use actual altitude, but we don't have it, so fake it
    var rand = Math.random();
    if (rand < 0.01)
      lastFakeAltitude--;
    else if (rand > 0.99)
      lastFakeAltitude++;
    statusBar.currentAltitude = "Altitude: " + lastFakeAltitude;
  }

  function updateHeading(newHeading)
  {
    statusBar.currentHeading = " Heading: " + newHeading
  }

  function updateMapRotation(newRotation)
  {
    northArrow.rotation = 360 - newRotation;
  }

  function updateBasemapList(newList)
  {
    btnBasemaps.scrollList = newList
  }

  function clearChemLightUI()
  {
    chemLightBar.clearChemLightUI()
  }

  function toggleMeasureButtons(state)
  {
    btnMeasureArea.visible = state;
    btnMeasureLine.visible = state;
    btnMeasurePointAndRadius.visible = state;
  }

  function activateMeasureArea(state) {
    if (state) {
      btnMeasureLine.setToggled(false)
      btnMeasurePointAndRadius.setToggled(false)
    }
  }

  function activateMeasureLine(state) {
    if (state) {
      btnMeasureArea.setToggled(false)
      btnMeasurePointAndRadius.setToggled(false)
    }
  }

  function activateMeasurePointAndRadius(state) {
    if (state) {
      btnMeasureLine.setToggled(false)
      btnMeasureArea.setToggled(false)
    }
  }

  id: window
  width: 1024
  height: 768
  minimumSize: "600x400"
  preferredSize: "800x600"

  ClassificationBar
  {
    id: classificationBar
    width: parent.width
    lblClassification: "Unclassified"
    classificationColor: Qt.rgba(0, 1, 0, 1)
  }

  ToggleButton
  {
    id: btnMenu
    buttonDefaultIcon: "../../icons/Menu-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Pressed.png"
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.margins: 3
    Component.onCompleted:
    {
      itemClicked.connect(menu.invoke)
      itemClicked.connect(mainMenu.clicked)
    }
    z: 2

    Menu
    {
      id: menu
      windowHeight: window.height - classificationBar.height - btnMenu.height - 15
      windowWidth: window.width / 4
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.bottom
      z: 2
      onMenuClosed:
      {
        while (stack.count() > 1)
        {
          stack.removePanel()
        }
      }

      Stack
      {
        id: stack
      }

      MainMenu
      {
        id: mainMenu
        stack: stack
        anchors.fill: parent
        onCloseMenu: btnMenu.setToggled(false)
      }
    }
  }

  PushButton
  {
    id: btnHome
    buttonDefaultIcon: "../../icons/Home-Normal.png"
    buttonActiveIcon: "../../icons/Home-Pressed.png"
    anchors.left: btnMenu.right
    anchors.bottom: parent.bottom
    anchors.margins: 3
    Component.onCompleted:
    {
      itemClicked.connect(window.homeClicked)
      itemClicked.connect(mainMenu.clicked)
    }
  }

  ToggleButton
  {
    id: btnEmergency
    objectName: "btnEmergency"
    buttonDefaultIcon: "../../icons/911-v2-Normal.png"
    buttonActiveIcon: "../../icons/911-v2-Pressed.png"
    anchors.top: classificationBar.bottom
    anchors.right: parent.right
    anchors.margins: 3
    Component.onCompleted:
    {
      itemClicked.connect(window.emergencyClicked)
      itemClicked.connect(mainMenu.clicked)
    }
  }

// Layer toggling, add back in if desired:
//  ScrollButton
//  {
//    id: btnBasemaps
//    scrollButtonDefaultIcon: "../../icons/Basemap-Normal.png"
//    anchors.top: classificationBar.bottom
//    anchors.topMargin: 10
//    anchors.left: parent.left
//    anchors.leftMargin: 5
//    visible: menu.state != "open"
//    Component.onCompleted:
//    {
//      itemChanged.connect(window.basemapChanged)
//      itemChanged.connect(mainMenu.clicked)
//    }
//  }

  StatusBar
  {
    id: statusBar
    width: 350
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.margins: 3
  }

  ChemLightBar
  {
    id: chemLightBar
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: parent.left
    anchors.margins: 3
    visible: menu.state != "open"
    Component.onCompleted:
    {
      chemLightSelected.connect(window.chemLightSelected)
      chemLightSelected.connect(mainMenu.clicked)
    }
  }

  ToggleButton
  {
    id: btnMeasure
    visible: false
    buttonDefaultIcon: "../../icons/Measure-Normal.png"
    buttonActiveIcon: "../../icons/Measure-Pressed.png"
    anchors.right: navControl.left
    anchors.bottom: parent.bottom
    anchors.margins: 3
    Component.onCompleted: {
      itemClicked.connect(window.toggleMeasureButtons)
      itemClicked.connect(mainMenu.clicked)
    }
  }

  ToggleButton
  {
    id: btnMeasureLine
    buttonDefaultIcon: "../../icons/Line-Normal.png"
    buttonActiveIcon: "../../icons/Line-Pressed.png"
    visible: false
    anchors.right: btnMeasure.left
    anchors.bottom: parent.bottom
    anchors.margins: 3
    Component.onCompleted: {
      itemClicked.connect(window.measureLineClicked)
      itemClicked.connect(window.activateMeasureLine)
      itemClicked.connect(mainMenu.clicked)
    }
  }

  ToggleButton
  {
    id: btnMeasureArea
    buttonDefaultIcon: "../../icons/Area-Normal.png"
    buttonActiveIcon: "../../icons/Area-Pressed.png"
    visible: false
    anchors.right: btnMeasureLine.left
    anchors.bottom: parent.bottom
    anchors.margins: 3
    Component.onCompleted: {
      itemClicked.connect(window.measureAreaClicked)
      itemClicked.connect(window.activateMeasureArea)
      itemClicked.connect(mainMenu.clicked)
    }
  }

  ToggleButton
  {
    id: btnMeasurePointAndRadius
    buttonDefaultIcon: "../../icons/PointAndRadius-Normal.png"
    buttonActiveIcon: "../../icons/PointAndRadius-Pressed.png"
    visible: false
    anchors.right: btnMeasureArea.left
    anchors.bottom: parent.bottom
    anchors.margins: 3
    Component.onCompleted: {
      itemClicked.connect(window.measurePointAndRadiusClicked)
      itemClicked.connect(window.activateMeasurePointAndRadius)
      itemClicked.connect(mainMenu.clicked)
    }
  }

  NavigationControl2
  {
    id: navControl
    objectName: "navControl"
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.margins: 3
    Component.onCompleted:
    {
      zoomInClicked.connect(window.zoomInClicked)
      zoomOutClicked.connect(window.zoomOutClicked)
      panClicked.connect(window.panClicked)
      clicked.connect(mainMenu.clicked)
    }
  }

  Image {
    id: northArrow
    source: "qrc:/Resources/icons/North-Arrow.png"
    anchors.bottomMargin: 25
    anchors.bottom: navControl.top
    anchors.horizontalCenter: navControl.horizontalCenter
    width: navControl.width * .75
    height: navControl.width * .75
    smooth: true
    fillMode: Image.PreserveAspectFit
  }

  IdentifyResultsPanel
  {
    id: idPanel
    objectName: "idPanel"
    visible: false
    width: parent.width / 4
    anchors.bottom: navControl.top
    anchors.top: btnEmergency.bottom
    anchors.right: parent.right
  }
}
