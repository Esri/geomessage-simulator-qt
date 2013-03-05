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

  id: activityPanel
  title: "Activity"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Attacking"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Defending"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Moving"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Stationary"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Cache"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Civilian"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Personnel Recovery"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Other"; onNoArgModelSignal: updateSelectedContent(text); }
  ]
}
