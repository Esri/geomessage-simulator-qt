import Qt 4.7
import "../../Controls"

Panel
{
  signal contentUpdated()

  property string selectedContent: "N/A"

  function updateSelectedContent(updatedText)
  {
    selectedContent = updatedText
    stack.removePanel()
    contentUpdated()
  }

  id: timePanel
  title: "Time"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Now"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Other"; onNoArgModelSignal: updateSelectedContent(text); }
  ]

}

