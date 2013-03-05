import Qt 4.7
import "../../Controls"

Panel
{
  signal recentSelected(variant value)

  function handleRecentSelected(value)
  {
    stack.removePanel()
    stack.removePanel()
    recentSelected(value)
  }

  function populateModel()
  {
    var modelElementList = []
    var modelComponent = Qt.createComponent("../../Controls/ModelObject.qml");

    // Dynamically create the model QtObjects
    var modelElement
    for (var i = 0; i < recentList.length; i++)
    {
      modelElement = modelComponent.createObject(recentPanel);
      modelElement.text = recentList[i]
      modelElement.type = "preview"
      modelElement.modelSignal.connect(recentList.handleRecentSelected)
      modelElementList.push(modelElement)
    }

    // Delete any pre-existing dynamically created model QtObjects
    for (var recentModel in recentPanel.model)
    {
      recentModel.destroy()
    }

     // Reassign the recent model with the new model list
    recentPanel.model = modelElementList
  }

  property variant recentList

  id: recentPanel
  title: "Recent"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  onRecentListChanged: populateModel()
  delegate: MultiDelegate { }
}


