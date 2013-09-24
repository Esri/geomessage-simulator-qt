import QtQuick 1.1
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

  id: unitPanel
  title: "Unit"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Conventional"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Irregular"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Coalition"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Host Nation"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "NGO"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Civilian"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Facility"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Other"; onNoArgModelSignal: updateSelectedContent(text); }
  ]
}

