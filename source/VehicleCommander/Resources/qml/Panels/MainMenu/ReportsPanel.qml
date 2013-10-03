import QtQuick 1.1
import "../../Controls"

Panel
{
  property Panel spotReportPanel

  id: reportsPanel
  title: "Reports"
  visible: false;
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Spot Report"; onNoArgModelSignal: stack.addPanel(spotReportPanel); }
    // Future report types would go here
  ]
}
