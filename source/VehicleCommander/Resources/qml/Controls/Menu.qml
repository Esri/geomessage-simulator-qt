import QtQuick 1.1

Rectangle
{
  property int windowHeight
  property int windowWidth
  property bool leftOfParent
  signal menuOpened()
  signal menuClosed()

  function invoke(activate)
  {
    if (activate)
    {
      state = "open"
      menuOpened()
    }
    else
    {
      state = "closed"
      menuClosed()
    }
  }

  id: menuPanel
  width: 50
  height: 50
  visible: false
  color: Qt.rgba(0.5, 0.5, 0.5, 0.75)
  border.color:  Qt.rgba(0, 0, 0, 0.5)
  border.width: 2
  radius: 10
  anchors.bottomMargin: 5
  state: "closed"
  leftOfParent: false

  states:
  [
    State
    {
      name: "open"
      PropertyChanges { target: menuPanel; width: windowWidth; height: windowHeight; visible: true }
      AnchorChanges {
        target: menuPanel;
        anchors.left: leftOfParent ? undefined : parent.left;
        anchors.right: leftOfParent ? parent.right : undefined;
        anchors.bottom: parent.top;
        anchors.top: undefined;
        anchors.horizontalCenter: undefined
      }
    },

    State
    {
      name: "closed"
      PropertyChanges { target: menuPanel; width: 50; height: 50; visible: false }
      AnchorChanges { target: menuPanel; anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.bottom; anchors.bottom: undefined; anchors.left: undefined; }
    }
  ]

  transitions:
  [
    Transition
    {
      to: "open"
      ParallelAnimation
      {
        AnchorAnimation { duration: 250 }
        NumberAnimation { target: menuPanel; property: "width"; to: windowWidth; duration: 250 }
        NumberAnimation { target: menuPanel; property: "height"; to: windowHeight; duration: 250 }
      }
    },

    Transition
    {
      to: "closed"

      SequentialAnimation
      {
        ParallelAnimation
        {
          AnchorAnimation { duration: 250 }
          NumberAnimation { target: menuPanel; property: "width"; to: 50; duration: 250 }
          NumberAnimation { target: menuPanel; property: "height"; to: 50; duration: 250 }
       }

       PropertyAction { target: menuPanel; property: "visible"; value: false }
     }
    }
  ]
}
