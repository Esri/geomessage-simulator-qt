import QtQuick 1.1

Rectangle
{
  property string lblClassification
  property color classificationColor: Qt.rgba(0, 1, 0, 1)

  id: classificationBar
  height: 20
  color: "black"
  anchors.margins: 0
  border.color: "black"
  border.width: 1

  Rectangle
  {
    height: 20
    width: lbl.width + 20
    color: classificationColor
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top

    Text
    {
      id: lbl
      text: classificationBar.lblClassification
      anchors.verticalCenter: parent.verticalCenter
      anchors.horizontalCenter: parent.horizontalCenter
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      font.family: "Courier New"
      font.bold: true
      font.pixelSize: 16
    }
  }
}
