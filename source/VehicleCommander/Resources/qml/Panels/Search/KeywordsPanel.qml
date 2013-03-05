import Qt 4.7
import "../../Controls"

Panel
{
  signal userAppliedKeywordSelected(variant value)

  function handleUserAppliedKeywordSelected(value)
  {
    stack.removePanel()
    userAppliedKeywordSelected(value)
    selectedContent = userAppliedKeywordList.length
  }

  function populateModel()
  {
    var modelElementList = []
    var modelComponent = Qt.createComponent("../../Controls/ModelObject.qml");

    // Dynamically create the model QtObjects
    for (var i = 0; i < userAppliedKeywordList.length; i++)
    {
      var modelElement = modelComponent.createObject(keywordsPanel);
      modelElement.text = userAppliedKeywordList[i]
      modelElement.modelSignal.connect(keywordsPanel.handleUserAppliedKeywordSelected)
      modelElementList.push(modelElement)
    }

    // Delete any pre-existing dynamically created model QtObjects
    for (var keywordModel in keywordsPanel.model)
    {
      keywordModel.destroy()
    }

    // Re-assign the keyword model with the new model list
    keywordsPanel.model = modelElementList
    selectedContent = userAppliedKeywordList.length
  }

  property Panel addKeywordsPanel
  property variant userAppliedKeywordList
  property string selectedContent: "0"

  id: keywordsPanel
  title: "Keywords"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  onUserAppliedKeywordListChanged: populateModel()

  PushButton
  {
    width: 150
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.margins: 15
    buttonText: "Add New Keyword"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    onItemClicked: stack.addPanel(addKeywordsPanel)
  }
}
