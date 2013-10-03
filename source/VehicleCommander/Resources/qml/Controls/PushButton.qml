import QtQuick 1.1

Rectangle
{
  signal itemClicked()

  function clicked()
  {
    if (parent.clicked)
      parent.clicked()
  }

  property string buttonText: ""
  property color buttonColor: "dimgrey"
  property color clickColor: "grey"
  property color textColor: "black"
  property string buttonDefaultIcon
  property string buttonActiveIcon
  property bool isPressed

  id: button
  color: buttonColor
  onButtonColorChanged: button.color = buttonColor
  onTextColorChanged: buttonText.color = textColor
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
    width: button.width
    height: button.height
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    anchors.fill: parent
    anchors.margins: 5
    color: textColor
    wrapMode: Text.Wrap
    font.family: "Courier New"
    font.pixelSize: 12

  }

  MouseArea
  {
    anchors.fill: parent
    onPressed: { defaultIcon.visible = false; activeIcon.visible = true; parent.color = clickColor; mouse.accepted = true; }
    onReleased: {
      parent.clicked()
      defaultIcon.visible = true; activeIcon.visible = false; parent.color = buttonColor; button.itemClicked(); mouse.accepted = true;
    }
    //onDoubleClicked: { mouse.accepted = true; }
  }
}

