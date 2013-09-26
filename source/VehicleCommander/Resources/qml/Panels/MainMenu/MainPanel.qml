import QtQuick 1.1
import "../../Controls"

Panel
{
  signal exitClicked()
  signal buttonClicked()

  property Panel reportsPanel
  property Panel overlaysPanel
  property Panel buddiesPanel
  property Panel observationsPanel
  property Panel optionsPanel
  property Panel visibilityPanel

  id: mainPanel
  title: "Main Menu"
  Component.onCompleted: buttonClicked.connect(clicked)
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Reports"; onNoArgModelSignal: stack.addPanel(reportsPanel); },
    ModelObject { text: "Overlays"; onNoArgModelSignal: stack.addPanel(overlaysPanel); },
    ModelObject { text: "Visibility"; onNoArgModelSignal: stack.addPanel(visibilityPanel);},
    ModelObject { text: "Buddies"; onNoArgModelSignal: stack.addPanel(buddiesPanel); },
    ModelObject { text: "Observations"; onNoArgModelSignal: stack.addPanel(observationsPanel); },
    ModelObject { text: "Options"; onNoArgModelSignal: stack.addPanel(optionsPanel); }
  ]

  PushButton
  {
    id: btnExit
    buttonText: "Quit Application"
    buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
    buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
    width: 150
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.margins: 10
    onItemClicked:
    {
      mainPanel.buttonClicked()
      mainPanel.exitClicked()
    }
  }
}
