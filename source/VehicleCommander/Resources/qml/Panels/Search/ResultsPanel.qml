import QtQuick 1.1
import "../../Controls"

Panel
{
  signal resultSelected(variant value)

  function handleResultSelected(value)
  {
    // Clear the result set
    resultList = []

    // Delete remaining dynamically created model QtObjects
    for (var keywordModel in resultsPanel.model)
    {
      keywordModel.destroy()
    }

    // Go back to the spot report panel
    while (stack.currentPanel().title != "Equipment")
    {
      stack.removePanel()
    }

    resultSelected(value)
  }

  function populateModel()
  {
    var modelElementList = []
    var modelComponent = Qt.createComponent("../../Controls/ModelObject.qml");

    endIndex = startIndex + displayCount
    if (resultList.length < endIndex)
      endIndex = resultList.length

    // Dynamically create the model QtObjects
    var modelElement
    var index
    for (var i = startIndex; i < endIndex; i++)
    {
      modelElement = modelComponent.createObject(resultsPanel);

      modelElement.text = resultList[i]
      modelElement.type = "preview"
      modelElement.modelSignal.connect(resultsPanel.handleResultSelected)
      modelElementList.push(modelElement)
    }

    // Delete any pre-existing dynamically created model QtObjects
    for (var keywordModel in resultsPanel.model)
    {
      keywordModel.destroy()
    }

     // Reassign the results model with the new model list
    resultsPanel.model = modelElementList
  }

  property variant resultList: []
  property int startIndex
  property int endIndex
  property int displayCount: 7

  id: resultsPanel
  objectName: "resultsPanel"
  title: "Results"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  onResultListChanged: { startIndex = 0; populateModel(); }
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
      if (endIndex < resultList.length)
      {
        startIndex = startIndex + displayCount
        populateModel()
      }
    }
  }

  Item
  {
    id: resultCount
    width: 150
    height: 50
    anchors.bottom: btnNext.top
    anchors.horizontalCenter: parent.horizontalCenter

    Text
    {
      id: lblResultCount
      text: (startIndex + 1) + " - " + endIndex + " of " + resultList.length + " Results"
      anchors.fill: parent
      font.pixelSize: 20
      color: "white"
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
    }
  }
}
