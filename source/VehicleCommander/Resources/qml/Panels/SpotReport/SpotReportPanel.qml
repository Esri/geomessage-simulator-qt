import QtQuick 1.1
import "../../Controls"

Panel
{
  signal reportReady()

  function updateEquipmentModel()
  {
    if (equipmentPanel.selectedContent == "N/A")
      equipmentModel.sideTextVisible = true
    else
      equipmentModel.sideTextVisible = false
  }

  function updateLocationModel()
  {
    if (locationPanel.selectedContent == "N/A")
      locationModel.sideTextVisible = true
    else
      locationModel.sideTextVisible = false
  }

  property Panel sizePanel
  property Panel activityPanel
  property Panel locationPanel
  property Panel unitPanel
  property Panel timePanel
  property Panel equipmentPanel

  id: spotReportPanel
  title: "SPOT Report"
  anchors.fill: parent
  visible: false
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  Component.onCompleted:
  {
// WARNING: There is a known issue that when
// this project's QML is run in the QML debugger, it does initialize
// in the correct order and the following properties are null
// For now, you will need to disable the QML debugger to run the debugger
// as a workaround (in Projects | Run | Debugger Settings)
    equipmentPanel.contentUpdated.connect(spotReportPanel.updateEquipmentModel)
    locationPanel.contentUpdated.connect(spotReportPanel.updateLocationModel)
  }

  model:
  [
    ModelObject { text: "Size: " + sizePanel.selectedContent; onNoArgModelSignal: stack.addPanel(sizePanel) },
    ModelObject { text: "Activity: " + activityPanel.selectedContent; onNoArgModelSignal: stack.addPanel(activityPanel) },
    ModelObject { id: locationModel; text: "Location: " + locationPanel.selectedContent; sideText: "*"; sideTextVisible: true; onNoArgModelSignal: stack.addPanel(locationPanel) },
    ModelObject { text: "Unit: " + unitPanel.selectedContent; onNoArgModelSignal: stack.addPanel(unitPanel) },
    ModelObject { text: "Time: " + timePanel.selectedContent; onNoArgModelSignal: stack.addPanel(timePanel) },
    ModelObject { id: equipmentModel; text: "Equipment: " + equipmentPanel.selectedContent; sideText: "*"; sideTextVisible: true; onNoArgModelSignal: stack.addPanel(equipmentPanel)}
  ]

  PushButton
  {
    id: btnSend
    width: 150
    buttonText: "Send"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.margins: 10
    onItemClicked: { spotReportPanel.reportReady(); spotReportPanel.goBack(); }
  }
}
