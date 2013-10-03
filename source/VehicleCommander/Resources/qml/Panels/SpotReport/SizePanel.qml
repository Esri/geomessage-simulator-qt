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

  id: sizePanel
  title: "Size"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "1"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "2"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "3"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "4"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "5"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "10"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "15"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "20"; onNoArgModelSignal: updateSelectedContent(text); },
    ModelObject { text: "Other"; onNoArgModelSignal: updateSelectedContent(text); }
  ]
}
