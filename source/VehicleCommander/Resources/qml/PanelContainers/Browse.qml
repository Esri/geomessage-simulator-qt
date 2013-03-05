import Qt 4.7
import "../Controls"
import "../Panels/Search"

Item
{
  signal updateAffiliation(variant affiliation)
  signal performSearch(variant searchText)
  signal updateCategory(variant catgory)
  signal keywordSelected(variant value)
  signal userAppliedKeywordSelected(variant value)
  signal clearClicked()

  property Stack stack
  property Panel panel
  property Panel resultsPanel

  id: browse
  panel: browsePanel

  BrowsePanel
  {
    id: browsePanel
    anchors.fill: parent
    stack: browse.stack
    onViewClicked: { stack.addPanel(resultsPanel); browse.performSearch(""); }
    affiliationPanel: affiliationPanel
    categoryPanel: categoryPanel
    keywordsPanel: keywords.panel
    Component.onCompleted: clearClicked.connect(browse.clearClicked)
  }

  AffiliationPanel
  {
    id: affiliationPanel
    stack: browse.stack
    onContentUpdated:
    {
      browse.updateAffiliation(selectedContent)
      stack.addPanel(categoryPanel)
    }
  }

  CategoryPanel
  {
    id: categoryPanel
    stack: browse.stack
    onCategoryUpdated:
    {
      browse.updateCategory(selectedContent)
      if (stack.currentPanel().title == "Affiliation")
        stack.removePanel()
    }
  }

  Keywords
  {
    id: keywords
    stack: browse.stack
    anchors.fill: parent
    Component.onCompleted:
    {
      keywordSelected.connect(browse.keywordSelected)
      userAppliedKeywordSelected.connect(browse.userAppliedKeywordSelected)
    }
  }
}
