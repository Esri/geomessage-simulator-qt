import QtQuick 1.1
import "../../Controls"

Panel
{
  signal visibilityAnalysisClicked()

  id: visibilityPanel
  title: "Visibility"
  visible: false
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
      ModelObject { text: "Visibility Analysis (from Vehicle)"; Component.onCompleted: modelSignal.connect(visibilityPanel.visibilityAnalysisClicked); }
  ]
}
