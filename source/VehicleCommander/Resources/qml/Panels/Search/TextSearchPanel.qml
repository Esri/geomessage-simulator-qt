import Qt 4.7
import "../../Controls"

Panel
{
  signal performSearch(variant searchText)

  function clearText()
  {
    inputField.noArgModelSignal()
  }

  function handleSearchClicked()
  {
    stack.addPanel(resultsPanel)
    performSearch(inputField.text);
  }

  property Panel resultsPanel

  title: "Text Search"
  anchors.fill: parent
  visible: false
  onGoBack: stack.removePanel()
  delegate: MultiDelegate { }
  model:
  [
    ModelObject { id: inputField; type: "textInput"; },
    ModelObject { text: "Search"; onNoArgModelSignal: handleSearchClicked() }
  ]
}
