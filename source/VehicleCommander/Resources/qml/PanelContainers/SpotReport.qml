import Qt 4.7
import "../Controls"
import "../Panels/SpotReport"
import "../Panels/Search"

Item
{
  signal goBack()
  signal reportReady(variant report)
  signal getPointFromMapClicked()

  function clicked()
  {
    if (parent.clicked)
      parent.clicked();
    if (parent.buttonClicked)
      parent.buttonClicked();
  }

  function prepareReport()
  {
    spotReport.reportReady(report)

    sizePanel.selectedContent = "N/A"
    activityPanel.selectedContent = "N/A"
    locationPanel.locationX = ""
    locationPanel.locationY = ""
    locationPanel.locationMGRS = ""
    locationPanel.selectedContent = "N/A"
    spotReportPanel.updateLocationModel()
    unitPanel.selectedContent = "N/A"
    timePanel.selectedContent = "N/A"
    equipmentSearch.panel.selectedContent = "N/A"
    spotReportPanel.updateEquipmentModel()
  }

  function updateLocation(x, y, mgrs)
  {
    locationPanel.updateLocation(x, y, mgrs)
  }

  property Stack stack
  property Panel panel

  id: spotReport
  panel: spotReportPanel
  resources:
  [
    QtObject
    {
      id: report
      property string size: sizePanel.selectedContent
      property string activity: activityPanel.selectedContent
      property string locationX: locationPanel.locationX
      property string locationY: locationPanel.locationY
      property string locationMGRS: locationPanel.locationMGRS
      property string unit: unitPanel.selectedContent
      property string time: timePanel.selectedContent
      property string equipment: equipmentSearch.panel.selectedContent
    }
  ]

  SpotReportPanel
  {
    id: spotReportPanel
    stack: spotReport.stack;
    sizePanel: sizePanel
    activityPanel: activityPanel
    locationPanel: locationPanel
    unitPanel: unitPanel
    timePanel: timePanel
    equipmentPanel: equipmentSearch.panel
    Component.onCompleted: { reportReady.connect(spotReport.prepareReport) }
  }

  SizePanel
  {
    id: sizePanel
    stack: spotReport.stack
    onContentUpdated: { stack.addPanel(activityPanel); }
  }

  ActivityPanel
  {
    id: activityPanel
    stack: spotReport.stack
    onContentUpdated: { stack.addPanel(locationPanel); }
  }

  LocationPanel
  {
    id: locationPanel
    stack: spotReport.stack
    onContentUpdated: { stack.addPanel(unitPanel); }
    Component.onCompleted: getPointFromMapClicked.connect(spotReport.getPointFromMapClicked)
  }

  UnitPanel
  {
    id: unitPanel
    stack: spotReport.stack
    onContentUpdated: { stack.addPanel(timePanel); }
  }

  TimePanel
  {
    id: timePanel
    stack: spotReport.stack
    onContentUpdated: { stack.addPanel(equipmentSearch.panel); }
  }

  EquipmentSearch
  {
    id: equipmentSearch
    stack: spotReport.stack
    anchors.fill: parent
  }
}
