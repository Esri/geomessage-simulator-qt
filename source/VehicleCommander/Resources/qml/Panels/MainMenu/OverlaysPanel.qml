import QtQuick 1.1
import "../../Controls"

Panel
{
  id: overlaysPanel
  title: "Overlays"
  visible: false;
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
    ModelObject
    {
      function doAction()
      {
        mainMenu.openCOA();
        mainMenu.clicked();
      }

      text: "COA";
      Component.onCompleted: modelSignal.connect(doAction);
    },
    ModelObject
    {
      function doAction()
      {
        mainMenu.openMPK();
        mainMenu.clicked();
      }

      text: "Map File";
      Component.onCompleted: modelSignal.connect(doAction);
    }
  ]
}
