import QtQuick 1.1
import "../../Controls"

Rectangle
{

  property variant resultsList: []
  property int resultsListIndex: 0

  function clicked()
  {
    if (parent.clicked)
      parent.clicked();
    if (parent.buttonClicked)
      parent.buttonClicked();
  }

  function show() {
    theListView.model.clear();

    if (0 < resultsList.length) {
      var result = resultsList[resultsListIndex];
      title.text = (undefined != result.attributes[result.displayFieldName]) ? result.attributes[result.displayFieldName] : "";
      var attKeys = result.attributeKeys;
      for (var i = 0; i < attKeys.length; i++) {
        theListView.model.append({"key": attKeys[i], "value": result.attributes[attKeys[i]]});
      }

      visible = true;
    }
  }

  anchors.margins: 5
  border.width: 2
  radius: 10
  border.color: Qt.rgba(0, 0, 0, 0.5)
  color: Qt.rgba(0.5, 0.5, 0.5, 0.5)
  onResultsListChanged: {
    resultsListIndex = 0;
    show();
  }

  MouseArea
  {
    anchors.fill: parent
    onReleased: parent.clicked()
  }

  PushButton
  {
    id: btnCloseIdPanel
    objectName: "btnCloseIdPanel"
    anchors.top: parent.top
    anchors.right: parent.right
    buttonDefaultIcon: "../../icons/X-Normal.png"
    buttonActiveIcon: "../../icons/X-Pressed.png"
    onItemClicked:
    {
      parent.clicked();
      parent.visible = false;
    }
  }

  PushButton
  {
    id: btnNextId
    objectName: "btnNextId"
    anchors.top: parent.top
    anchors.right: btnCloseIdPanel.left
    buttonDefaultIcon: "../../icons/Forward-Normal.png"
    buttonActiveIcon: "../../icons/Forward-Pressed.png"
    onItemClicked:
    {
      resultsListIndex++;
      if (resultsListIndex >= resultsList.length) {
        resultsListIndex = 0;
      }
      show();
      parent.clicked();
    }
  }

  PushButton
  {
    id: btnPrevId
    objectName: "btnPrevId"
    anchors.top: parent.top
    anchors.right: btnNextId.left
    buttonDefaultIcon: "../../icons/Back-Normal.png"
    buttonActiveIcon: "../../icons/Back-Pressed.png"
    onItemClicked:
    {
      resultsListIndex--;
      if (0 > resultsListIndex) {
        resultsListIndex = resultsList.length - 1;
      }
      show();
      parent.clicked();
    }
  }

  Text {
    id: title
    objectName: "title"
    text: "The Title"
    anchors.margins: 5
    anchors.top: btnPrevId.bottom
    anchors.left: parent.left
    anchors.right: counter.left
    wrapMode: Text.WordWrap
    font.pointSize: 16
    color: "#FFFFFF"
  }

  Text {
    id: counter
    objectName: "counter"
    text: (resultsListIndex + 1) + " of " + resultsList.length
    anchors.margins: 5
    width: 50;
    anchors.top: btnPrevId.bottom
    anchors.right: parent.right
    font.pointSize: 16
    color: "#FFFFFF"
    horizontalAlignment: Text.AlignRight
  }

  Rectangle {
    id: titleUnderline
    objectName: "titleUnderline"
    color: "#FFFFFF"
    anchors.margins: 5
    anchors.top: counter.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: 2
  }

  ListView {
    id: theListView
    objectName: "theListView"
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: titleUnderline.bottom
    anchors.bottom: parent.bottom
    anchors.margins: 5
    model: ListModel {}
    delegate: Row {
      property variant _key: key;
      property variant _value: value;
      width: parent.width
      Text {
        width: parent.width / 3.0;
        wrapMode: Text.Wrap
        font.pointSize: 16
        color: "#DDDDDD"
        text: _key + ":"
      }
      Text {
        width: parent.width * 2.0 / 3.0;
        font.pointSize: 16
        color: "#FFFFFF"
        wrapMode: Text.Wrap
        text: _value
      }
    }
  }
}
