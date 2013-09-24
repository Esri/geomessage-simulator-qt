import QtQuick 1.1
import "../../Controls"

Panel
{
  signal contentUpdated()

  function updateSelectedContent(updatedText)
  {
    selectedContent = updatedText
    stack.removePanel();
    contentUpdated()
  }

  property string selectedContent: "N/A"
  property Panel searchPanel

  id: equipmentPanel
  title: "Equipment"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Missile Launcher H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "Grenade Launcher H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "Howitzer H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "Armored Personnel Carrier H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "Ground Vehicle H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "Armored Tank H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "Rifle H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); },
    ModelObject { text: "IED H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); }
    //ModelObject { text: "Supply H"; type: "preview"; onNoArgModelSignal: equipmentPanel.updateSelectedContent(text); }
  ]

  PushButton
  {
    width: 150
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.margins: 15
    buttonText: "Other"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    onItemClicked: stack.addPanel(searchPanel)
  }
}
