import QtQuick 1.1
import "../Controls"

Rectangle
{
  id: navControl2
  width: 115
  height: 280
  radius: 10
  border.width: 2
  border.color: Qt.rgba(0, 0, 0, 0.5)
  color: Qt.rgba(0.5, 0.5, 0.5, 0.5)
  anchors.margins: 5
  Component.onCompleted:
  {
    zoomInClicked.connect(navControl2.clicked())
    zoomOutClicked.connect(navControl2.clicked())
    panClicked.connect(navControl2.clicked())
  }

  signal zoomInClicked()
  signal zoomOutClicked()
  signal panClicked(string direction)
  signal clicked()

  MouseArea
  {
    anchors.fill: parent
    onReleased: parent.clicked()
  }

  PushButton
  {
    id: zoomIn
    buttonDefaultIcon: "../../icons/Nav-Controls-Plus-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-Plus-Pressed.png"
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    onItemClicked: navControl.zoomInClicked()
  }

  PushButton
  {
    id: panUp
    buttonDefaultIcon: "../../icons/Nav-Controls-North-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-North-Pressed.png"
    anchors.top: zoomIn.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    onItemClicked: navControl.panClicked("up")
  }

  PushButton
  {
    id: panLeft
    buttonDefaultIcon: "../../icons/Nav-Controls-West-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-West-Pressed.png"
    anchors.top: panUp.bottom
    anchors.left: parent.left
    onItemClicked: navControl.panClicked("left")
  }

  PushButton
  {
    id: panRight
    buttonDefaultIcon: "../../icons/Nav-Controls-East-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-East-Pressed.png"
    anchors.top: panUp.bottom
    anchors.right: parent.right
    onItemClicked: navControl.panClicked("right")
  }

  PushButton
  {
    id: panDown
    buttonDefaultIcon: "../../icons/Nav-Controls-South-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-South-Pressed.png"
    anchors.top: panLeft.bottom
    anchors.horizontalCenter: panUp.horizontalCenter
    onItemClicked: navControl.panClicked("down")
  }

  PushButton
  {
    id: zoomOut
    buttonDefaultIcon: "../../icons/Nav-Controls-Minus-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-Minus-Pressed.png"
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    onItemClicked: navControl.zoomOutClicked()
  }
}
