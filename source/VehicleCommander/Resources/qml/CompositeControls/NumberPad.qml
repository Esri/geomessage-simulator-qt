import Qt 4.7
import "../Controls"

Rectangle
{
  id: keyPad
  width: childrenRect.width + 10
  height: childrenRect.height + 10
  color: "dimgrey"
  border.color: "white"
  border.width: 2
  radius: 10
  anchors.margins: 5

  Rectangle
  {
    id: viewArea
    width: buttonGrid.width
    height: 50
    border.color: "black"
    border.width: 2
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.margins: 5
    radius: 5
    clip: true

//    Rectangle
//    {
//      id: cursor_rect
//      width: 2
//      height: 20
//      color: "black"
//      anchors.right: parent.right
//      anchors.verticalCenter: parent.verticalCenter
//      anchors.margins: 3

//      PropertyAnimation on opacity
//      {
//        //easing.type: Easing.OutSine
//        easing.type: Easing.Linear
//        loops: Animation.Infinite
//        from: 0
//        to: 1.0
//        duration: 1000
//      }
//    }

    Text
    {
      id: textArea
      anchors.fill: parent
      anchors.margins: 5
      horizontalAlignment: Text.AlignRight
      verticalAlignment: Text.AlignVCenter
      font.pixelSize: 20
      font.letterSpacing: 2
    }

  }

  Grid
  {
    id: buttonGrid
    width: childrenRect.width
    height: childrenRect.height
    columns: 3
    rows: 3
    spacing: 5
    anchors.top: viewArea.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.margins: 10

    Repeater
    {
      property list<QtObject> objectList:
      [
        QtObject { property string text: "7"; },
        QtObject { property string text: "8"; },
        QtObject { property string text: "9"; },
        QtObject { property string text: "4"; },
        QtObject { property string text: "5"; },
        QtObject { property string text: "6"; },
        QtObject { property string text: "1"; },
        QtObject { property string text: "2"; },
        QtObject { property string text: "3"; }
      ]

      model: objectList

      PushButton
      {
        buttonText: model.modelData.text
        buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
        buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
        onItemClicked: { textArea.text = textArea.text + buttonText }
      }
    }
  }

  PushButton
  {
    id: btnClear
    buttonText: "Clear"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    anchors.left: parent.left
    anchors.top: buttonGrid.bottom
    onItemClicked: textArea.text = ""
  }

  PushButton
  {
    id: btnZero
    buttonText: "0"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    anchors.horizontalCenter: buttonGrid.horizontalCenter
    anchors.top: buttonGrid.bottom
    onItemClicked: textArea.text = textArea.text + buttonText
  }

  PushButton
  {
    id: btnEnter
    buttonText: "Enter"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    anchors.right: parent.right
    anchors.top: buttonGrid.bottom
    onItemClicked: {}
  }
}
