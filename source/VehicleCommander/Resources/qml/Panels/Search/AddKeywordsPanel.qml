import QtQuick 1.1
import "../../Controls"

Panel
{
  signal keywordSelected(variant value)

  function handleKeywordSelected(keyword)
  {
    stack.removePanel()
    stack.removePanel()
    keywordSelected(keyword)
    startIndex = 0
  }

  function populateModel()
  {
    var modelElementList = []
    var modelComponent = Qt.createComponent("../../Controls/ModelObject.qml");

    endIndex = startIndex + displayCount
    if (keywordList.length < endIndex)
      endIndex = keywordList.length

    // Dynamically create the model QtObjects
    for (var i = startIndex; i < endIndex; i++)
    {
      var modelElement = modelComponent.createObject(addKeywordsPanel);
      modelElement.text = keywordList[i]
      modelElement.modelSignal.connect(addKeywordsPanel.handleKeywordSelected)
      modelElementList.push(modelElement)
    }

    // Delete any pre-existing dynamically created model QtObjects
    for (var keywordModel in addKeywordsPanel.model)
    {
      keywordModel.destroy()
    }

    // Re-assign the keyword model with the new model list
    addKeywordsPanel.model = modelElementList
  }

  property int startIndex
  property int endIndex
  property int displayCount: 7
  property string selectedContent
  property variant keywordList: []

  id: addKeywordsPanel
  objectName: "addKeywordsPanel"
  title: "Add Keywords"
  onGoBack: { stack.removePanel(); startIndex = 0; }
  anchors.fill: parent
  visible: false
  onKeywordListChanged: populateModel()
  delegate: MultiDelegate { }

  PushButton
  {
    id: btnPrevious
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.margins: 10
    buttonDefaultIcon: "../../icons/Nav-Controls-West-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-West-Pressed.png"
    onItemClicked:
    {
      if (startIndex == 0)
        return

      if (startIndex > displayCount)
        startIndex = startIndex - displayCount
      else
        startIndex = 0

      populateModel()
    }
  }

  PushButton
  {
    id: btnNext
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.margins: 10
    buttonDefaultIcon: "../../icons/Nav-Controls-East-Normal.png"
    buttonActiveIcon: "../../icons/Nav-Controls-East-Pressed.png"
    onItemClicked:
    {
      if (endIndex < keywordList.length)
      {
        startIndex = startIndex + displayCount
        populateModel()
      }
    }
  }

  Item
  {
    id: keywordCount
    width: 150
    height: 50
    anchors.bottom: btnNext.top
    anchors.horizontalCenter: parent.horizontalCenter

    Text
    {
      id: lblKeywordCount
      text: (startIndex + 1) + " - " + endIndex + " of " + addKeywordsPanel.keywordList.length + " Keywords"
      anchors.fill: parent
      font.pixelSize: 20
      color: "white"
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
    }
  }
}

