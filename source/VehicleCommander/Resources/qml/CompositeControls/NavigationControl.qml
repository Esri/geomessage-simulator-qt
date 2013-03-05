import Qt 4.7
import "../Controls"

Item
{
  id: navControl
  width: 250
  height: 250

  signal zoomInClicked()
  signal zoomOutClicked()
  signal panUpClicked()
  signal panDownClicked()
  signal panLeftClicked()
  signal panRightClicked()

  Rectangle
  {
    id: outerArea
    width: 182
    height: 182
    radius: 91
    color: Qt.rgba(0, 0, 0, 0)
    border.width: 4
    border.color: "black"
    anchors.centerIn: parent
  }

  Rectangle
  {
    id: middleArea
    width: 174
    height: 174
    radius: 87
    border.width: 8
    border.color: "grey" //Qt.rgba(0.5, 0.5, 0.5, 0.75)
    color: Qt.rgba(0, 0, 0, 0)
    anchors.centerIn: parent
  }

  Rectangle
  {
    id: innerArea
    width: 170
    height: 170
    radius: 85
    border.width: 2
    border.color: "black"
    color: Qt.rgba(0, 0, 0, 0)
    anchors.centerIn: parent

    PushButton
    {
      id: btnUp
      radius: 25
      buttonText: "^"
      anchors.verticalCenter: parent.top
      anchors.horizontalCenter: parent.horizontalCenter
      z: 2
      onItemClicked: { navControl.panUpClicked() }
    }

    PushButton
    {
      id: btnRight
      radius: 25
      buttonText: ">"
      anchors.verticalCenter: parent.verticalCenter
      anchors.horizontalCenter: parent.right
      z: 2
      onItemClicked: navControl.panRightClicked()
    }

    PushButton
    {
      id: btnDown
      radius: 25
      buttonText: "v"
      anchors.verticalCenter: parent.bottom
      anchors.horizontalCenter: parent.horizontalCenter
      z: 2
      onItemClicked: navControl.panDownClicked()
    }

    PushButton
    {
      id: btnLeft
      radius: 25
      buttonText: "<"
      anchors.verticalCenter: parent.verticalCenter
      anchors.horizontalCenter: parent.left
      z: 2
      onItemClicked: navControl.panLeftClicked()
    }

    PushButton
    {
      id: zoomIn
      buttonText: "+"
      anchors.top: btnUp.bottom
      anchors.horizontalCenter: parent.horizontalCenter
      z: 1
      onItemClicked: navControl.zoomInClicked()
    }

    PushButton
    {
      id: zoomOut
      buttonText: "-"
      anchors.bottom: btnDown.top
      anchors.horizontalCenter: parent.horizontalCenter
      z: 1
      onItemClicked: navControl.zoomOutClicked()
    }
  }
}
