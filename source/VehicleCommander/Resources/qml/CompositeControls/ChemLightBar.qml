import QtQuick 1.1
import "../Controls"

Rectangle
{
  signal chemLightSelected(string color)

  function clearChemLightUI()
  {
    selector.state = ""
  }

  id: chemLightBar
  width: 60
  height: 225
  radius: 10
  border.width: 2
  border.color: Qt.rgba(0, 0, 0, 0.5)
  color: Qt.rgba(0.5, 0.5, 0.5, 0.5)
  anchors.margins: 5
  onFocusChanged: selector.visible = chemLight.activeFocus

  Rectangle
  {
    id: selector
    width: 52
    height: 52
    radius: 10
    color: Qt.rgba(0,0,0,0)
    border.width: 4
    border.color: "black"
    visible: false
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.top
    z: 1

    states:
    [
      State
      {
        name: "red"
        AnchorChanges { target: selector; anchors.verticalCenter: redLight.verticalCenter; anchors.horizontalCenter: redLight.horizontalCenter }
        PropertyChanges { target: selector; visible: true }
      },

      State
      {
        name: "yellow"
        AnchorChanges { target: selector; anchors.verticalCenter: yellowLight.verticalCenter; anchors.horizontalCenter: yellowLight.horizontalCenter }
        PropertyChanges { target: selector; visible: true }
      },

      State
      {
        name: "green"
        AnchorChanges { target: selector; anchors.verticalCenter: greenLight.verticalCenter; anchors.horizontalCenter: greenLight.horizontalCenter }
        PropertyChanges { target: selector; visible: true }
      },

      State
      {
        name: "blue"
        AnchorChanges { target: selector; anchors.verticalCenter: blueLight.verticalCenter; anchors.horizontalCenter: blueLight.horizontalCenter }
        PropertyChanges { target: selector; visible: true }
      }
    ]

    transitions:
    [
      Transition { AnchorAnimation { duration: 250; easing.type: Easing.Linear; } }
    ]
  }

  PushButton
  {
    id: redLight
    buttonDefaultIcon: "../../icons/ChemLights-Red-Normal.png"
    buttonActiveIcon: "../../icons/ChemLights-Red-Pressed.png"
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    onItemClicked: { selector.state = "red"; chemLightBar.chemLightSelected("red") }
    z: 2
  }

  PushButton
  {
    id: yellowLight
    buttonDefaultIcon: "../../icons/ChemLights-Yellow-Normal.png"
    buttonActiveIcon: "../../icons/ChemLights-Yellow-Pressed.png"
    anchors.top: redLight.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    onItemClicked: { selector.state = "yellow"; chemLightBar.chemLightSelected("yellow") }
    z: 2
  }

  PushButton
  {
    id: greenLight
    buttonDefaultIcon: "../../icons/ChemLights-Green-Normal.png"
    buttonActiveIcon: "../../icons/ChemLights-Green-Pressed.png"
    anchors.top: yellowLight.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    onItemClicked: { selector.state = "green"; chemLightBar.chemLightSelected("green") }
    z: 2
  }

  PushButton
  {
    id: blueLight
    buttonDefaultIcon: "../../icons/ChemLights-Blue-Normal.png"
    buttonActiveIcon: "../../icons/ChemLights-Blue-Pressed.png"
    anchors.top: greenLight.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    onItemClicked: { selector.state = "blue"; chemLightBar.chemLightSelected("blue") }
    z: 2
  } 
}


