import QtQuick 1.1

Item
{
  signal goBack()
  signal clicked()

  property string title
  property Stack stack
  property list<QtObject> model
  property Component delegate

stack: stack

  function populateMenuPath()
  {
    var pathString = "";
    var array = stack.getArray()
    for (var i = 0; i < array.length; ++i)
    {
      pathString = pathString + array[i].title + " \\ "
    }

    return pathString
  }

  id: panel
  onVisibleChanged: { if (visible == true) menuPath.text = panel.populateMenuPath() }
  onClicked:
  {
    if (parent.clicked)
      parent.clicked();
  }

  Item
  {
    id: titleBar
    height: btnBack.height
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: btnBack.left

    Text
    {
      id: name
      height: btnBack.height
      text: panel.title
      anchors.fill: parent
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignBottom
      font.pixelSize: 26
      font.bold: true
      color: "white"
    }
  }

  MouseArea
  {
    id: mouseArea
    anchors.fill: parent
    onClicked:
    {
      panel.clicked();
    }
  }

  PushButton
  {
    id: btnBack
    buttonDefaultIcon: "../../icons/Back-Normal.png"
    buttonActiveIcon: "../../icons/Back-Pressed.png"
    anchors.right: parent.right
    anchors.top: parent.top
    onItemClicked:
    {
      goBack()
      mainMenu.clicked()
    }
  }

  Rectangle
  {
    id: headerBorder
    height: 2
    width: panel.width - 10
    anchors.top: btnBack.bottom
    anchors.horizontalCenter: panel.horizontalCenter
    anchors.margins: 5
    color: "white"
  }

  Text
  {
    id: menuPath
    text: ""
    font.family: "Courier New"
    font.pixelSize: 10
    color: "white"
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignVCenter
    anchors.top: headerBorder.bottom
    anchors.left: headerBorder.left
    anchors.margins: 5
    width: panel.width - 10
    wrapMode: Text.Wrap
  }

  Column
  {
    width: childrenRect.width
    height: childrenRect.height
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    spacing: 10

    Repeater
    {
      model: panel.model
      delegate: panel.delegate
    }
  }
}
