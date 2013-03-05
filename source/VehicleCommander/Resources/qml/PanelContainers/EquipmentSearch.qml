import Qt 4.7
import "../Panels/SpotReport"
import "../Controls"

Item
{
  signal contentUpdated(string content)

  property Stack stack
  property Panel panel

  function clicked()
  {
    if (parent.clicked)
      parent.clicked();
    if (parent.buttonClicked)
      parent.buttonClicked();
  }

  id: equipmentSearch
  panel: equipmentPanel

  EquipmentPanel
  {
    id: equipmentPanel;
    stack: equipmentSearch.stack;
    searchPanel: search.panel
    anchors.fill: parent
    onContentUpdated: equipmentSearch.contentUpdated(data)
  }

  Search
  {
    id: search
    stack: equipmentSearch.stack
    anchors.fill: parent
    Component.onCompleted: resultSelected.connect(equipmentPanel.updateSelectedContent)
  }
}

