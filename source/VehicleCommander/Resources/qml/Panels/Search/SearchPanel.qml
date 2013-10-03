import QtQuick 1.1
import "../../Controls"

Panel
{
  signal contentUpdated()

  property Panel browsePanel
  property Panel textSearchPanel
  property Panel recentPanel

  id: searchPanel
  title: "Search"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Browse"; onNoArgModelSignal: stack.addPanel(browsePanel); },
    ModelObject { text: "Search"; onNoArgModelSignal: stack.addPanel(textSearchPanel); },
    ModelObject { text: "Recent"; onNoArgModelSignal: stack.addPanel(recentPanel); }
  ]
}
