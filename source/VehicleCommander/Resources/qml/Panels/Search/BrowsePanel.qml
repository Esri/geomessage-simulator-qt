import QtQuick 1.1
import "../../Controls"

Panel
{
  signal viewClicked()
  signal clearClicked()

  property string viewLabelText
  property Panel affiliationPanel
  property Panel categoryPanel
  property Panel keywordsPanel

  id: browsePanel
  title: "Browse"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Affiliation: " + affiliationPanel.selectedContent; onNoArgModelSignal: stack.addPanel(affiliationPanel); },
    ModelObject { text: "Category: " + categoryPanel.selectedContent; onNoArgModelSignal: stack.addPanel(categoryPanel); },
    ModelObject { text: "Applied Keywords: " + keywordsPanel.selectedContent; onNoArgModelSignal: stack.addPanel(keywordsPanel); },
    ModelObject { text: "Clear"; onNoArgModelSignal: browsePanel.clearClicked(); }
  ]

  Item
  {
    id: resultCount
    width: 150
    height: 50
    anchors.bottom: btnView.top
    anchors.horizontalCenter: parent.horizontalCenter

    Text
    {
      id: lblResultCount
      text: browsePanel.viewLabelText
      anchors.fill: parent
      font.pixelSize: 20
      color: "white"
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
    }
  }

  PushButton
  {
    id: btnView
    width: 150
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    buttonText: "View Results"
    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.margins: 15
    onItemClicked: browsePanel.viewClicked()
  }
}
