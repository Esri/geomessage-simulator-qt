import QtQuick 1.1

QtObject
{
  signal modelSignal(variant data);
  signal noArgModelSignal()

  function clicked()
  {
    if (parent.clicked)
      parent.clicked();
    if (parent.buttonClicked)
      parent.buttonClicked();
  }

  Component.onCompleted:
  {
    noArgModelSignal.connect(modelObject.clicked);
    modelSignal.connect(modelObject.clicked);
  }

  property string text
  property string sideText: ""
  property bool sideTextVisible: false
  property string type: "push"
  property bool startEnabled: false
  property Item obj
  property color backgroundColor: "dimgrey"
  property color foregroundColor: "black"

  id: modelObject
  objectName: "modelObject"
}
