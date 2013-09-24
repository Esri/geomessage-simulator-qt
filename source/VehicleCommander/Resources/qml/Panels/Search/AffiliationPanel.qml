import QtQuick 1.1
import "../../Controls"

Panel
{
  signal contentUpdated()

  function updateSelectedContent(updatedText)
  {
    selectedContent = updatedText
    stack.removePanel()
    contentUpdated()
  }

  property string selectedContent: "None"

  id: affiliationPanel
  title: "Affiliation"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Hostile"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Neutral"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Unknown"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Friendly"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "All"; onNoArgModelSignal: updateSelectedContent(text); }
  ]
}
