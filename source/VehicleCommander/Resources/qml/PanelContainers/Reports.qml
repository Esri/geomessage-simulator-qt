import QtQuick 1.1
import "../Panels/MainMenu"
import "../Controls"

Item
{
  property Stack stack
  property Panel panel

  function clicked()
  {
    if (parent.clicked)
      parent.clicked();
    if (parent.buttonClicked)
      parent.buttonClicked();
  }

  id: reports
  panel: reportsPanel

  ReportsPanel
  {
    id: reportsPanel;
    stack: reports.stack;
    anchors.fill: parent
    spotReportPanel: spotReport.panel
  }

  SpotReport
  {
    id:spotReport
    objectName: "spotReport"
    stack: reports.stack
    anchors.fill: parent
  }
}
