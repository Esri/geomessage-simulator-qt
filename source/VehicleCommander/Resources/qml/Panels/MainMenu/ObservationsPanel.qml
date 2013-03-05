import Qt 4.7
import "../../Controls"

Panel
{
  signal receiveSpotReportsClicked(bool state)
  signal showObservationsClicked(bool state)

  id: observationsPanel
  title: "Observations"
  visible: false
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "Show Observations"; startEnabled: true; type: "toggle"; Component.onCompleted: modelSignal.connect(observationsPanel.showObservationsClicked); },
    ModelObject { text: "Receive Spot Reports"; startEnabled: true; type: "toggle"; Component.onCompleted: modelSignal.connect(observationsPanel.receiveSpotReportsClicked); }
  ]
}
