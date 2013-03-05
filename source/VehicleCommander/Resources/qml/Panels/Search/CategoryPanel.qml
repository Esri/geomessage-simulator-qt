import Qt 4.7
import "../../Controls"

Panel
{
  signal categoryUpdated(variant value)

  function handleCategorySelected(category)
  {
    selectedContent = category
    stack.removePanel()
    categoryPanel.categoryUpdated(category)
  }

  function populateModel()
  {
    var modelElementList = []
    var modelComponent = Qt.createComponent("../../Controls/ModelObject.qml");

    // Dynamically create the model QtObjects
    for (var i = 0; i < categoryList.length; i++)
    {
      var modelElement = modelComponent.createObject(categoryPanel);
      modelElement.text = categoryList[i]
      modelElement.modelSignal.connect(categoryPanel.handleCategorySelected)
      modelElementList.push(modelElement)
    }

    // Delete any pre-existing dynamically created model QtObjects
    for (var categoryModel in categoryPanel.model)
    {
      categoryModel.destroy()
    }

    // Reassign the keyword model with the new model list
    categoryPanel.model = modelElementList
  }

  property string selectedContent: "None"
  property variant categoryList

  id: categoryPanel
  title: "Categories"
  onGoBack: stack.removePanel()
  anchors.fill: parent
  visible: false
  onCategoryListChanged: populateModel()
  delegate: MultiDelegate { }
}

