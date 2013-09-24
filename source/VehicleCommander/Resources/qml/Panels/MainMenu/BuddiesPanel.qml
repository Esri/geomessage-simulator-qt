import QtQuick 1.1
import "../../Controls"

Panel
{
  signal receiveBuddyPositionsClicked(bool state)
  signal showBuddiesClicked(bool state)

  id: buddiesPanel
  title: "Buddies"
  visible: false
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Show Buddies"; startEnabled: true; type: "toggle"; Component.onCompleted: modelSignal.connect(buddiesPanel.showBuddiesClicked); },
    ModelObject { text: "Receive Buddy Positions"; startEnabled: true; type: "toggle"; Component.onCompleted: modelSignal.connect(buddiesPanel.receiveBuddyPositionsClicked); }
  ]
}
