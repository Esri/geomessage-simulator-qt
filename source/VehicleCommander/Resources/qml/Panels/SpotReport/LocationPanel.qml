import Qt 4.7
import "../../Controls"

Panel
{
  signal getPointFromMapClicked()
  signal contentUpdated()

  property string locationX
  property string locationY
  property string locationMGRS
  property string selectedContent: "N/A"

  function updateLocation(x, y, mgrs)
  {
    locationX = x
    locationY = y
    locationMGRS = mgrs
    //selectedContent = locationX.substring(0, 6) + ", " + locationY.substring(0, 6)
    selectedContent = mgrs
    locationPanel.stack.removePanel()
    contentUpdated()
  }

  id: locationPanel
  objectName: "locationPanel"
  title: "Location"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { text: "From Map"; Component.onCompleted: noArgModelSignal.connect(locationPanel.getPointFromMapClicked); },
    ModelObject { text: "Manual MGRS Coordinate"; },
    ModelObject { text: "Manual Lat/Lon Coordinate"; },
    ModelObject { text: "Offset from Me"; }
  ]
}

