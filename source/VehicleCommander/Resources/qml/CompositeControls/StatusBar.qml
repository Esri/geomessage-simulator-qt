import Qt 4.7

Rectangle
{
  property variant currentLocation: "Location: "
  property variant currentHeading : " Heading: "
  property variant currentAltitude: "Altitude: "
  property variant currentSpeed   : "   Speed: "

  id: myLocationBar
  width: 200
  height: 50
  radius: 5
  border.width: 2
  border.color: Qt.rgba(0, 0, 0, 0.5)
  color: Qt.rgba(0.5, 0.5, 0.5, 0.5)

  Rectangle
  {
    id: innerBar
    anchors.centerIn: parent
    color: "dimgrey"
    border.color: "black"
    border.width: 1
    width: parent.width - 10
    height: parent.height - 10
    radius: 5

    Image
    {
      id: defaultIcon
      anchors.fill: parent
      source:  "../../icons/Status-Bar.png"
      fillMode: Image.Stretch
    }

    Text
    {
      id: myCoordinates
      text: myLocationBar.currentLocation
      height: 20
      anchors.left: parent.left
      anchors.top: parent.top
      anchors.margins: 4
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignTop
      color: "black"
      font.family: "Courier New"
      font.pixelSize: 12
    }

    Text
    {
      id: myHeading
      text: myLocationBar.currentHeading
      height: 20
      anchors.left: parent.left
      anchors.bottom: parent.bottom
      anchors.margins: 4
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignBottom
      color: "black"
      font.family: "Courier New"
      font.pixelSize: 12
    }

    Text
    {
      visible: false
      id: myAltitude
      text: myLocationBar.currentAltitude
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
      anchors.horizontalCenterOffset: parent.width / 4
      anchors.margins: 4
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignTop
      color: "black"
      font.family: "Courier New"
      font.pixelSize: 12
    }

    Text
    {
      visible: false
      id: mySpeed
      text: myLocationBar.currentSpeed
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.horizontalCenterOffset: parent.width / 4
      anchors.margins: 4
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignBottom
      color: "black"
      font.family: "Courier New"
      font.pixelSize: 12
    }
  }
}
