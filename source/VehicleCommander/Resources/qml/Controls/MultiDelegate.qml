import QtQuick 1.1

Item
{
  signal clicked()

  function determineDelegate(type)
  {
    if (type == "toggle")
      return toggleDelegate
    else if (type == "textInput")
      return textInputDelegate
    else if (type == "preview")
      return previewDelegate

    return pushDelegate
  }

  id: multiDelegate
  width: childrenRect.width
  height: childrenRect.height
  Component.onCompleted:
  {
    //TODO sometimes there is no parent. In fact, lots of times! Figure this out
    if (parent && parent.clicked)
      clicked.connect(parent.clicked)
  }

  Component
  {
    id: toggleDelegate

    ToggleButton
    {
      buttonText: model.modelData.text
      buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
      buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
      width: 150
      Component.onCompleted: { itemClicked.connect(model.modelData.modelSignal); setToggled(model.modelData.startEnabled); }
    }
  }

  Component
  {
    id: pushDelegate

    Item
    {
      width: childrenRect.width
      height: childrenRect.height
      anchors.horizontalCenter: parent.horizontalCenter

      PushButton
      {
        id: btn
        buttonText: model.modelData.text
        buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
        buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
        width: 150
        textColor: model.modelData.foregroundColor
        buttonColor: model.modelData.backgroundColor
        Component.onCompleted: itemClicked.connect(model.modelData.noArgModelSignal)
        onItemClicked:
        {
          multiDelegate.clicked()
          model.modelData.modelSignal(model.modelData.text)
        }
      }

      Text
      {
        text: model.modelData.sideText;
        font.pixelSize: 20
        color: "red"
        anchors.verticalCenter: btn.verticalCenter;
        anchors.right: btn.left;
        anchors.margins: 5
        visible: model.modelData.sideTextVisible
      }
    }
  }

  Component
  {
    id: previewDelegate

    Item
    {
      width: 200
      height: 50

      PushButton
      {
        id: btn
        buttonText: model.modelData.text
        buttonDefaultIcon: "../../icons/Menu-Button-Normal.png"
        buttonActiveIcon: "../../icons/Menu-Button-Pressed.png"
        anchors.horizontalCenter: parent.horizontalCenter
        width: 150
        textColor: model.modelData.foregroundColor
        buttonColor: model.modelData.backgroundColor
        onItemClicked: model.modelData.modelSignal(model.modelData.text)
        Component.onCompleted:
        {
          itemClicked.connect(model.modelData.noArgModelSignal);
          itemClicked.connect(multiDelegate.clicked)
        }
      }

      Image
      {
        //TODO this width and height never actually make it to the dictionary image provider
        //  (DictionaryImageProvider). For now, I'll hardcode it in
        //  DictionaryImageProvider::requestImage, but at some point we want to figure out how to make
        //  the provider get the size.
        width: 50
        height: 50
        source: "image://dictionary/" + model.modelData.text
        anchors.verticalCenter: btn.verticalCenter;
        anchors.right: btn.left;
      }
    }
  }

  Component
  {
    id: textInputDelegate

    Rectangle
    {
      width: 150
      height: 50
      border.color: "white"
      border.width: 2
      radius: 10
      clip: true

      TextInput
      {
        function clearText()
        {
          inputField.text = ""
        }

        id: inputField
        focus: true
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 20
        onTextChanged: { model.modelData.text = text; }
        Component.onCompleted: model.modelData.noArgModelSignal.connect(clearText)
      }
    }
  }

  Loader
  {
    id: itemDisplay
    sourceComponent: determineDelegate(model.modelData.type)
  }
}
