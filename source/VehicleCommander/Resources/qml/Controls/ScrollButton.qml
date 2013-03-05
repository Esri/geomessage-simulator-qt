import Qt 4.7

Item
{
  property int currentIndex
  property variant scrollList: ["", "", ""]
  property string text1
  property string text2
  property string text3
  property string scrollButtonDefaultIcon

  signal itemChanged(string name)

  function stateChange()
  {
    ++currentIndex

    currentIndex = currentIndex % scrollList.length

    if (state == "one")
    {
      text2 = scrollList[currentIndex]
      state = "two"
    }
    else if (state == "two")
    {
      text3 = scrollList[currentIndex]
      state = "three"
    }
    else if (state == "three")
    {
      text1 = scrollList[currentIndex]
      state = "one"
    }

    itemChanged(scrollList[currentIndex])
  }

  id: scrollButton
  width: 100
  height: 50
  state: "one"
  onScrollListChanged: { state = "one"; currentIndex = 0 }
  currentIndex: 0
  text1: scrollList[0]

  states:
  [
    State
    {
      name: "one"
      AnchorChanges { target: element1; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: undefined; anchors.verticalCenter: parent.verticalCenter }
      AnchorChanges { target: element2; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: undefined; anchors.bottom: parent.top }
      AnchorChanges { target: element3; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: undefined; anchors.top: parent.bottom }
      PropertyChanges { target: element1; visible: true }
      PropertyChanges { target: element2; visible: false }
      PropertyChanges { target: element3; visible: false }
    },

    State
    {
      name: "two"
      AnchorChanges { target: element1; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: undefined; anchors.top: parent.bottom }
      AnchorChanges { target: element2; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: undefined; anchors.verticalCenter: parent.verticalCenter }
      AnchorChanges { target: element3; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: undefined; anchors.bottom: parent.top }
      PropertyChanges { target: element1; visible: false }
      PropertyChanges { target: element2; visible: true }
      PropertyChanges { target: element3; visible: false }
    },

    State
    {
      name: "three"
      AnchorChanges { target: element1; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: undefined; anchors.bottom: parent.top }
      AnchorChanges { target: element2; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: undefined; anchors.top: parent.bottom }
      AnchorChanges { target: element3; anchors.horizontalCenter: parent.horizontalCenter; anchors.bottom: undefined; anchors.verticalCenter: parent.verticalCenter }
      PropertyChanges { target: element1; visible: false }
      PropertyChanges { target: element2; visible: false }
      PropertyChanges { target: element3; visible: true }
    }
  ]

  transitions:
  [
    Transition
    {
      SequentialAnimation
      {
        AnchorAnimation { duration: 250; easing.type: Easing.InOutBack; }
        PropertyAction { property: "visible"; }
      }
    },

    Transition
    {
      to: "one"
      SequentialAnimation
      {
        PropertyAction { target: element1; property: "visible"; value: true }
        AnchorAnimation { duration: 250; easing.type: Easing.InOutBack; }
        PropertyAction { targets: [element2, element3]; property: "visible"; value: false }
      }
    },

    Transition
    {
      to: "two"
      SequentialAnimation
      {
        PropertyAction { target: element2; property: "visible"; value: true }
        AnchorAnimation { duration: 250; easing.type: Easing.InOutBack; }
        PropertyAction { targets: [element1, element3]; property: "visible"; value: false }
      }
    },

    Transition
    {
      to: "three"
      SequentialAnimation
      {
        PropertyAction { target: element3; property: "visible"; value: true }
        AnchorAnimation { duration: 250; easing.type: Easing.InOutBack; }
        PropertyAction { targets: [element1, element2]; property: "visible"; value: false }
      }
    }
  ]

  Rectangle
  {
    id: outerBox
    width: parent.width - 10
    height: parent.height - 10
    border.width: 1
    border.color: Qt.rgba(0, 0, 0, 0.5)
    color: Qt.rgba(0.5, 0.5, 0.5, 0.5)
    radius: 5
    clip: true
    anchors.centerIn: parent
  }

  Rectangle
  {
    id: middleBox
    anchors.fill: outerBox
    anchors.margins: 5
    color: Qt.rgba(0.5, 0.5, 0.5, 0.5)
    radius: 5
    clip: true
  }

  Rectangle
  {
    id: innerBox
    color: "dimgrey"
    radius: 5
    border.width: 1
    border.color: "black"
    anchors.fill: middleBox
    clip: true

    Image
    {
      id: defaultIcon
      anchors.fill: parent
      source: scrollButton.scrollButtonDefaultIcon
      fillMode: Image.Stretch
    }

    Text
    {
      id: element1
      text: scrollButton.text1
      font.pixelSize: 10
      font.bold: true
      font.letterSpacing: 1
      color: "black"
    }

    Text
    {
      id: element2
      text: scrollButton.text2
      font.pixelSize: 10
      font.bold: true
      font.letterSpacing: 1
      color: "black"
    }

    Text
    {
      id: element3
      text: scrollButton.text3
      font.pixelSize: 10
      font.bold: true
      font.letterSpacing: 1
      color: "black"
    }

    MouseArea
    {
      anchors.fill: parent
      onClicked: scrollButton.stateChange()
    }
  }
}
