import QtQuick 1.1

Rectangle
{
  signal itemClicked(bool toggleState)

  function setToggled(newState)
  {
    if (newState == true)
    {
      state = "on"
      isPressed = true
      itemClicked(true)
    }
    else
    {
      state = ""
      isPressed = false
      itemClicked(false)
    }
  }

  property string buttonText: ""
  property bool isPressed: false
  property string buttonDefaultIcon
  property string buttonActiveIcon

  id: button
  color: "dimgrey"
  border.color: "black"
  border.width: 2
  radius: 5
  width: 50
  height: 50
  anchors.margins: 5

  Image
  {
    id: defaultIcon
    anchors.fill: parent
    source: button.buttonDefaultIcon
    fillMode: Image.Stretch
  }

  Image
  {
    id: activeIcon
    anchors.fill: parent
    source: button.buttonActiveIcon
    fillMode: Image.Stretch
    visible: false
  }

  Text
  {
    text: buttonText
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    anchors.fill: parent
    anchors.margins: 5
    color: "black"
    wrapMode: Text.Wrap
    font.family: "Courier New"
    font.pixelSize: 12
  }

  MouseArea
  {
    id: mouseArea
    anchors.fill: parent
    onClicked: { button.setToggled(button.state == ""); mouse.accepted = true; }
  }

  states:
  [
    State
    {
      name: "on"
      //PropertyChanges { target:button ; color: "green" }
      PropertyChanges { target:activeIcon; visible: true }
      PropertyChanges { target:defaultIcon ; visible: false }
    }
  ]
}
