import Qt 4.7
import "../../Controls"

Panel
{
  signal showMeToggled(bool state)
  signal followMeToggled(bool state)
  signal sendMyPositionToggled(bool state)
  signal resetMapClicked()
  signal aboutMeClicked()

  id: optionsMenu
  title: "Options"
  visible: false
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "About Me"; Component.onCompleted: noArgModelSignal.connect(optionsMenu.aboutMeClicked); },
    ModelObject { text: "Show Me"; type: "toggle"; Component.onCompleted: modelSignal.connect(optionsMenu.showMeToggled); },
    ModelObject { text: "Follow Me"; type: "toggle"; Component.onCompleted: modelSignal.connect(optionsMenu.followMeToggled); },
    ModelObject { text: "Send My Location"; startEnabled: true; type: "toggle"; Component.onCompleted: modelSignal.connect(optionsMenu.sendMyPositionToggled); },
    ModelObject { text: "Reset Map"; Component.onCompleted: noArgModelSignal.connect(optionsMenu.resetMapClicked); }
  ]
}
